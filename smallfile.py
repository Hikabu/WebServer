import os
import random
import string
import sys

def generate_random_string(length):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def generate_large_file(filename, size_in_mb):
    chunk_size = 1024 * 1024  # 1 MB
    total_chunks = size_in_mb  # Number of 1MB chunks

    with open(filename, 'w') as file:
        for _ in range(total_chunks):
            chunk = generate_random_string(chunk_size)
            formatted_chunk = '\n'.join(chunk[i:i+100] for i in range(0, len(chunk), 100))
            file.write(formatted_chunk + '\n')

    actual_size = os.path.getsize(filename)
    print(f"File '{filename}' created.")
    print(f"Actual size: {actual_size / (1024**2):.2f} MB")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <size_in_mb>")
        sys.exit(1)
    
    size_in_mb = int(sys.argv[1])
    filename = f"small_file_{size_in_mb}MB.png"
    generate_large_file(filename, size_in_mb)
