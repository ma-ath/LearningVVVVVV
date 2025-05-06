#include "GameController.h"
#include "Screen.h"
#include <iostream>


GameController::GameController() {
}

void GameController::pressKey(SDL_Keycode key) {
    simulateKeyEvent(key, SDL_KEYDOWN);
}

void GameController::releaseKey(SDL_Keycode key) {
    simulateKeyEvent(key, SDL_KEYUP);
}

SDL_Surface* GameController::capture_screen(Screen& screen)
{
    if (!screen.m_renderer)
    {
        SDL_Log("Renderer is not initialized!");
        return NULL;
    }

    int width, height;
    SDL_GetRendererOutputSize(screen.m_renderer, &width, &height);

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (!surface)
    {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        return NULL;
    }

    if (SDL_RenderReadPixels(screen.m_renderer, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch) != 0)
    {
        SDL_Log("Failed to read pixels: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    return surface;
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
