#!/usr/bin/env python3

import unittest
import requests
import socket
import sys
import logging
from datetime import datetime

# Set up colorful logging if running in a terminal
def setup_colored_logging():
    class ColorFormatter(logging.Formatter):
        GREEN = '\033[92m'
        RED = '\033[91m'
        YELLOW = '\033[93m'
        BLUE = '\033[94m'
        RESET = '\033[0m'
        
        def format(self, record):
            if record.levelno == logging.INFO:
                color = self.BLUE
            elif record.levelno == logging.ERROR:
                color = self.RED
            elif record.levelno == logging.WARNING:
                color = self.YELLOW
            else:
                color = self.RESET
                
            if record.levelname == 'PASS':
                color = self.GREEN
            elif record.levelname == 'FAIL':
                color = self.RED
                
            record.msg = f"{color}{record.msg}{self.RESET}"
            return super().format(record)

# Set up logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[logging.StreamHandler(sys.stdout)]
)
logger = logging.getLogger(__name__)

# Add custom logging levels
logging.addLevelName(25, 'PASS')
logging.addLevelName(35, 'FAIL')

# If we're in a terminal, use colored output
if sys.stdout.isatty():
    for handler in logger.handlers:
        handler.setFormatter(ColorFormatter('%(asctime)s - %(levelname)s - %(message)s'))

class WebServerTest(unittest.TestCase):
    BASE_URL = "http://localhost:8080"
    TEST_TIMEOUT = 5
    test_results = {'passed': 0, 'failed': 0, 'details': []}

    @classmethod
    def setUpClass(cls):
        logger.info(f"{'='*20} STARTING TESTS {'='*20}")
        logger.info(f"Target server: {cls.BASE_URL}")
        
        try:
            response = requests.get(cls.BASE_URL, timeout=cls.TEST_TIMEOUT)
            logger.info(f"Server is running and responded with status code: {response.status_code}")
        except requests.ConnectionError:
            logger.error(f"Could not connect to {cls.BASE_URL}. Is the server running?")
            sys.exit(1)
        except requests.Timeout:
            logger.error(f"Server at {cls.BASE_URL} is not responding within {cls.TEST_TIMEOUT} seconds")
            sys.exit(1)

    def record_result(self, test_name, passed, details=""):
        result = {
            'test_name': test_name,
            'passed': passed,
            'details': details
        }
        self.test_results['details'].append(result)
        if passed:
            self.test_results['passed'] += 1
            logger.log(25, f"PASSED: {test_name}")  # 25 is our custom PASS level
        else:
            self.test_results['failed'] += 1
            logger.log(35, f"FAILED: {test_name} - {details}")  # 35 is our custom FAIL level

    def test_1_status_codes(self):
        """Test various HTTP status codes"""
        test_name = "Status Codes Test"
        try:
            test_cases = [
                ("/nonexistent", 404, "Non-existent path"),
                ("/", 200, "Root path"),
                ("/test_directory", 403, "Directory listing")
            ]
            
            all_passed = True
            details = []
            
            for path, expected_code, description in test_cases:
                logger.info(f"Checking {description} ({path})")
                response = requests.get(f"{self.BASE_URL}{path}", timeout=self.TEST_TIMEOUT)
                if response.status_code != expected_code:
                    all_passed = False
                    details.append(f"{path} returned {response.status_code}, expected {expected_code}")
                
            self.record_result(test_name, all_passed, "; ".join(details) if details else "All status codes correct")
            
        except Exception as e:
            self.record_result(test_name, False, str(e))

    def test_2_chunked_encoding(self):
        """Test handling of malformed chunked encoding"""
        test_name = "Chunked Encoding Test"
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(self.TEST_TIMEOUT)
            s.connect(("localhost", 8080))
            
            request = ("POST /upload HTTP/1.1\r\n"
                      "Host: localhost\r\n"
                      "Transfer-Encoding: chunked\r\n\r\n"
                      "5\r\nabc")  # Malformed chunk
            
            logger.info("Sending malformed chunked request")
            s.send(request.encode())
            response = s.recv(4096).decode()
            
            expected_responses = ["400 Bad Request", "500 Internal Server Error"]
            passed = any(resp in response for resp in expected_responses)
            
            details = f"Got response: {response.splitlines()[0] if response else 'No response'}"
            self.record_result(test_name, passed, details)
            
        except Exception as e:
            self.record_result(test_name, False, str(e))
        finally:
            s.close()

    def test_3_put_request(self):
        """Test PUT request functionality"""
        test_name = "PUT Request Test"
        try:
            test_file = "/test.txt"
            content = "Test content"
            
            logger.info(f"Sending PUT request to {test_file}")
            put_response = requests.put(f"{self.BASE_URL}{test_file}", data=content, timeout=self.TEST_TIMEOUT)
            
            get_response = requests.get(f"{self.BASE_URL}{test_file}", timeout=self.TEST_TIMEOUT)
            
            passed = put_response.status_code in [201, 204] and get_response.status_code == 200
            details = f"PUT returned {put_response.status_code}, GET returned {get_response.status_code}"
            
            self.record_result(test_name, passed, details)
            
        except Exception as e:
            self.record_result(test_name, False, str(e))

    @classmethod
    def tearDownClass(cls):
        logger.info(f"{'='*20} TEST SUMMARY {'='*20}")
        logger.info(f"Total tests run: {cls.test_results['passed'] + cls.test_results['failed']}")
        if cls.test_results['passed'] > 0:
            logger.log(25, f"Tests passed: {cls.test_results['passed']}")
        if cls.test_results['failed'] > 0:
            logger.log(35, f"Tests failed: {cls.test_results['failed']}")
        
        logger.info(f"{'='*20} DETAILED RESULTS {'='*20}")
        for result in cls.test_results['details']:
            level = 25 if result['passed'] else 35
            logger.log(level, f"{result['test_name']}: {'PASS' if result['passed'] else 'FAIL'}")
            if result['details']:
                logger.info(f"  Details: {result['details']}")

def run_tests():
    suite = unittest.TestSuite()
    suite.addTest(WebServerTest('test_1_status_codes'))
    suite.addTest(WebServerTest('test_2_chunked_encoding'))
    suite.addTest(WebServerTest('test_3_put_request'))
    
    runner = unittest.TextTestRunner(verbosity=0)  # Suppress unittest's output
    result = runner.run(suite)
    
    return len(result.failures) + len(result.errors)

if __name__ == '__main__':
    start_time = datetime.now()
    
    try:
        failures = run_tests()
        end_time = datetime.now()
        duration = end_time - start_time
        
        logger.info(f"{'='*20} TEST EXECUTION COMPLETE {'='*20}")
        logger.info(f"Total execution time: {duration.total_seconds():.2f} seconds")
        
        if failures > 0:
            sys.exit(1)
        sys.exit(0)
        
    except KeyboardInterrupt:
        logger.error("Tests interrupted by user")
        sys.exit(1)
    except Exception as e:
        logger.error(f"Unexpected error during testing: {str(e)}")
        sys.exit(1)