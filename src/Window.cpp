#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Window.hpp"
#include "utils.hpp"

Window::Window(const std::string &title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        panic("couldn't init SDL: " + std::string(SDL_GetError()));
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        panic("couldn't init SDL_image: " + std::string(IMG_GetError()));
    }

     if (TTF_Init() == -1) {
        panic("couldn't init SDL_ttf: " + std::string(TTF_GetError()));
    }

    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        panic("couldn't create renderer: " + std::string(SDL_GetError()));
    }

    // Make the use of transparancy possible
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

    this->should_close = false;
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->renderer);
}

void Window::present() {
    SDL_RenderPresent(this->renderer);
}

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
