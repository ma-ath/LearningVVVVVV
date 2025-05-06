import time

from GameController import GameController
from Messages import Messages


if __name__ == "__main__":
    print("Starting Game Controller...")
    game_controller = GameController()

    while True:
        # Example of sending commands
        game_controller.send_command(Messages.PRESS_LEFT)
        time.sleep(0.5)
        game_controller.send_command(Messages.RELEASE_LEFT)
        time.sleep(0.5)
        game_controller.send_command(Messages.PRESS_RIGHT)
        time.sleep(0.5)
        game_controller.send_command(Messages.RELEASE_RIGHT)
        time.sleep(0.5)
