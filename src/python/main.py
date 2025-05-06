import time
from GameController import GameController
from Messages import Messages


if __name__ == "__main__":
    print("Starting Game Controller...")
    game_controller = GameController()

    try:
        while True:
            print("1. Control Game")
            print("2. Stream Game Screen")
            print("3. Exit")
            choice = input("Enter your choice: ")

            if choice == "1":
                # Example of sending commands
                game_controller.send_command(Messages.PRESS_LEFT)
                time.sleep(0.5)
                game_controller.send_command(Messages.RELEASE_LEFT)
                time.sleep(0.5)
                game_controller.send_command(Messages.PRESS_RIGHT)
                time.sleep(0.5)
                game_controller.send_command(Messages.RELEASE_RIGHT)
                time.sleep(0.5)
            elif choice == "2":
                print("Streaming game screen. Press 'q' to stop.")
                game_controller.stream_screen()
            elif choice == "3":
                print("Exiting...")
                break
            else:
                print("Invalid choice. Please try again.")
    except KeyboardInterrupt:
        print("\nExiting...")
