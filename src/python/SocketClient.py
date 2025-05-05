import socket

SERVER_HOST = '127.0.0.1'  # localhost
SERVER_PORT = 8000         # Must match VVVVVV_SERVER_PORT in C++

def main():
    try:
        # Create a TCP socket
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((SERVER_HOST, SERVER_PORT))
            print(f"Connected to server at {SERVER_HOST}:{SERVER_PORT}")

            # Send a message
            message = "Hello from Python client!"
            sock.sendall(message.encode('utf-8'))
            print(f"Sent: {message}")

            # Optionally receive a response (not implemented in C++ server currently)
            # response = sock.recv(1024)
            # print(f"Received: {response.decode('utf-8')}")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == '__main__':
    main()
