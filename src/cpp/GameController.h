#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SDL2/SDL.h>
#include "Screen.h"

class GameController
{
public:
    GameController();
    void pressKey(SDL_Keycode key);
    void releaseKey(SDL_Keycode key);
    SDL_Surface* capture_screen(Screen& screen);

private:
    void simulateKeyEvent(SDL_Keycode key, Uint32 type);
};

#endif // GAMECONTROLLER_H_
