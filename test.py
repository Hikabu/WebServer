import socket
import select
import time
import sys
import psutil

def find_free_port(start_port, end_port):
    for port in range(start_port, end_port + 1):
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.bind(("", port))
            s.close()
            return port
        except OSError:
            continue
    return None

def get_process_using_port(port):
    for conn in psutil.net_connections():
        if conn.laddr.port == port:
            try:
                return psutil.Process(conn.pid)
            except psutil.NoSuchProcess:
                return None
    return None

def monitor_webserver(host, port, timeout=60):
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((host, port))
    except OSError as e:
        if e.errno == 98:  # Address already in use
            print(f"Error: Port {port} is already in use.")
            process = get_process_using_port(port)
            if process:
                print(f"It's being used by process: {process.name()} (PID: {process.pid})")
            free_port = find_free_port(port + 1, port + 100)
            if free_port:
                print(f"Suggested free port: {free_port}")
            else:
                print("No free ports found in the range.")
            return
        else:
            raise

    server_socket.listen(5)
    
    sockets_list = [server_socket]
    clients = {}
    
    print(f"Monitoring server on {host}:{port}")
    
    while True:
        try:
            read_sockets, _, exception_sockets = select.select(sockets_list, [], sockets_list, 1)
            
            for notified_socket in read_sockets:
                if notified_socket == server_socket:
                    client_socket, client_address = server_socket.accept()
                    print(f"New connection from {client_address}")
                    sockets_list.append(client_socket)
                    clients[client_socket] = time.time()
                else:
                    try:
                        data = notified_socket.recv(1024)
                        if data:
                            print(f"Received data from {notified_socket.getpeername()}")
                            clients[notified_socket] = time.time()
                        else:
                            print(f"Closed connection from {notified_socket.getpeername()}")
                            sockets_list.remove(notified_socket)
                            del clients[notified_socket]
                    except:
                        print(f"Closed connection from {notified_socket.getpeername()}")
                        sockets_list.remove(notified_socket)
                        del clients[notified_socket]
            
            current_time = time.time()
            for client_socket, last_activity in list(clients.items()):
                if current_time - last_activity > timeout:
                    print(f"Connection {client_socket.getpeername()} timed out")
                    sockets_list.remove(client_socket)
                    del clients[client_socket]
                    client_socket.close()
            
            for notified_socket in exception_sockets:
                print(f"Exception on socket {notified_socket.getpeername()}")
                sockets_list.remove(notified_socket)
                del clients[notified_socket]
                
        except KeyboardInterrupt:
            print("\nShutting down the server...")
            for s in sockets_list:
                s.close()
            sys.exit(0)

if __name__ == "__main__":
    HOST = "127.0.0.1"  # localhost
    PORT = 8083  # You can change this to your desired port
    monitor_webserver(HOST, PORT)