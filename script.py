import os

# Constants
filename = '20GB_file.txt'
file_size_gb = 20  # Desired file size in GB
file_size_bytes = file_size_gb * (1024**3)  # Convert GB to bytes
chunk_size = 1024 * 1024  # 1 MB chunk size

# Create a file and write random data
with open(filename, 'wb') as f:
    bytes_written = 0
    while bytes_written < file_size_bytes:
        f.write(os.urandom(chunk_size))
        bytes_written += chunk_size
        print(f"{bytes_written / (1024**3):.2f} GB written", end="\r")

print(f"File '{filename}' of size {file_size_gb} GB generated successfully.")
