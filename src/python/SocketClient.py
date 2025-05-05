import socket

from Messages import Messages


class SocketClient:
    def __init__(self, host: str = '127.0.0.1', port: int = 8000):
        self.socket = None
        self.host = host
        self.port = port

    def Connect(self) -> None:
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            print(f"Connected to game server at {self.host}:{self.port}")
        except Exception as e:
            print(f"An error occurred: {e}")

    def Send(self, message: bytes) -> None:
        if self.socket:
            self.socket.sendall(message)
        else:
            print("Socket is not connected.")

    def Close(self):
        if self.socket:
            self.socket.close()
            print("Socket closed.")
