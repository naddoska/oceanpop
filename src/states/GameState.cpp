#include "GameState.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameState::GameState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, "Level failed", "Press the confirm button to restart")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;

    loadLevel();
}

GameState::~GameState() {
    delete(this->board);
}

void GameState::update() {
    if (this->board->isCompleted() && !this->completed) {
        theme.pause();
        sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed) {
        theme.pause();
        sounds->play(Sound::FAILED);
        this->failed = true;
    }

    if (!this->paused && !this->completed &&  !this->failed) {
        this->theme.update();
        this->board->update();
    }
}

void GameState::handleEvents(std::vector<Event> events) {
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
        } else if (this->completed) {
            if (event == Event::CONFIRM) {
                this->completed = false;
                this->theme.next();
                this->board->loadLevel(this->position.x, this->position.y, this->width, this->height, this->moves, ++this->required_matches, ++this->level, ++this->seed);
            }
        } else if (this->failed) {
            if (event == Event::CONFIRM) {
                this->failed = false;
                this->board->reset();
                theme.unpause();
            }
        }
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->completed) {
        win_screen.draw(renderer);
    } else if (this->failed) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board->draw(renderer);
    }
}

void GameState::loadLevel() {
    this->width = BOARD_WIDTH;
    this->height = BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    this->moves = 10;
    this->required_matches = 12;
    this->level = 1;
    this->seed = this->level;

    this->board = new BoardManager(
        renderer,
        this->fonts,
        this->sounds,
        this->position.x,
        this->position.y,
        this->width,
        this->height,
        this->moves,
        this->required_matches,
        this->level,
        this->seed
    );
}

SDL_Point GameState::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameState::getNextState() {
    return State::MENU;
}
