from enum import Enum

class Messages(Enum):    
    PRESS_UP: int =                       0x00
    RELEASE_UP: int =                     0x01
    PRESS_DOWN: int =                     0x02
    RELEASE_DOWN: int =                   0x03
    PRESS_LEFT: int =                     0x04
    RELEASE_LEFT: int =                   0x05
    PRESS_RIGHT: int =                    0x06
    RELEASE_RIGHT: int =                  0x07
    PRESS_Z: int =                        0x08
    RELEASE_Z: int =                      0x09
    PRESS_ESC: int =                      0x0A
    RELEASE_ESC: int =                    0x0B
    GAME_CONTROLLER_REQUEST_SCREEN: int = 0x0C
    GAME_CONTROLLER_STOP_SCREEN: int =    0x0D
