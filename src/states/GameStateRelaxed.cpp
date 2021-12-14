#include "GameStateRelaxed.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameStateRelaxed::GameStateRelaxed(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;

    loadLevel();
}

GameStateRelaxed::~GameStateRelaxed() {
    delete(this->board);
}

void GameStateRelaxed::update() {
    if (!this->paused) {
        this->theme.update();
        this->board->update();
    }
}

void GameStateRelaxed::handleEvents(std::vector<Event> events) {
    if (!this->paused) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (event == Event::MENU) {
            this->paused = !(this->paused);
            return;
        }

        if (this->paused) {
            if (event == Event::CONFIRM) {
                this->done = true;
            } else if (event == Event::CANCEL) {
                this->paused = false;
            }
        }
    }
}

void GameStateRelaxed::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board->draw(renderer);
    }
}

void GameStateRelaxed::loadLevel() {
    this->width = BOARD_WIDTH;
    this->height = BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    int moves = 0;
    int required_matches = 0;
    int level = 0;
    int seed = 0;

    this->board = new BoardManager(
        renderer,
        this->fonts,
        this->sounds,
        this->position.x,
        this->position.y,
        this->width,
        this->height,
        moves,
        required_matches,
        level,
        seed
    );
}

SDL_Point GameStateRelaxed::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameStateRelaxed::getNextState() {
    return State::MENU;
}