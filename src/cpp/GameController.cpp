#include "GameController.h"
#include <iostream>


GameController::GameController() {
}

void GameController::pressKey(SDL_Keycode key) {
    simulateKeyEvent(key, SDL_KEYDOWN);
}

void GameController::releaseKey(SDL_Keycode key) {
    simulateKeyEvent(key, SDL_KEYUP);
}

void GameController::simulateKeyEvent(SDL_Keycode key, Uint32 type) {
    std::cout << "Simulated key event: " << SDL_GetKeyName(key)
              << " " << (type == SDL_KEYDOWN ? "pressed" : "released") << std::endl;

    SDL_Event event;
    event.type = type;
    event.key.keysym.sym = key;
    event.key.state = (type == SDL_KEYDOWN) ? SDL_PRESSED : SDL_RELEASED;
    SDL_PushEvent(&event);
}
