from SocketClient import SocketClient
from Messages import Messages


class GameController:
    def __init__(self):
        self.socket_client = SocketClient()
        self.socket_client.Connect()

    def send_command(self, command: Messages) -> None:
        self.socket_client.Send(command.value.to_bytes(1, 'big'))

    def __del__(self):
        self.socket_client.Close()