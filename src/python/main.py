import time

from GameController import GameController
from Messages import Messages


if __name__ == "__main__":
    print("Starting Game Controller...")
    game_controller = GameController()

    while True:
        # Example of sending commands
        game_controller.send_command(Messages.UP)
        time.sleep(1)
        game_controller.send_command(Messages.DOWN)
        time.sleep(1)
        game_controller.send_command(Messages.LEFT)
        time.sleep(1)
        game_controller.send_command(Messages.RIGHT)
        time.sleep(1)