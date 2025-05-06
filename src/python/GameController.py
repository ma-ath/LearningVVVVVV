import struct
import cv2
import numpy as np
from SocketClient import SocketClient
from Messages import Messages


class GameController:
    def __init__(self):
        self.socket_client = SocketClient()
        self.socket_client.Connect()

    def send_command(self, command: Messages) -> None:
        self.socket_client.Send(command.value.to_bytes(1, 'big'))

    def stream_screen(self) -> None:
        """
        Request the game screen stream and display it in real-time.
        """
        try:
            # Send the command to start streaming
            self.socket_client.Send(Messages.GAME_CONTROLLER_REQUEST_SCREEN.value.to_bytes(1, 'big'))

            while True:
                # Receive the size of the PNG data
                size_data = self.socket_client.socket.recv(8)
                if not size_data:
                    break
                png_size = struct.unpack('Q', size_data)[0]

                # Receive the PNG data
                png_data = b""
                while len(png_data) < png_size:
                    packet = self.socket_client.socket.recv(4096)
                    if not packet:
                        break
                    png_data += packet

                # Decode the PNG data and display it
                np_data = np.frombuffer(png_data, np.uint8)
                image = cv2.imdecode(np_data, cv2.IMREAD_COLOR)
                if image is not None:
                    cv2.imshow("Game Screen", image)
                    if cv2.waitKey(1) & 0xFF == ord('q'):  # Press 'q' to quit
                        break
                else:
                    print("Failed to decode image")

        except Exception as e:
            print(f"Error while streaming screen: {e}")

        finally:
            # Send the command to stop streaming
            self.socket_client.Send(Messages.GAME_CONTROLLER_STOP_SCREEN.value.to_bytes(1, 'big'))
            cv2.destroyAllWindows()

    def __del__(self):
        self.socket_client.Close()