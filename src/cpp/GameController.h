#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SDL2/SDL.h>


class GameController {

public:
    GameController();  // private constructor
    void pressKey(SDL_Keycode key);
    void releaseKey(SDL_Keycode key);

private:
    void simulateKeyEvent(SDL_Keycode key, Uint32 type);
};

#endif  // GAMECONTROLLER_H_
