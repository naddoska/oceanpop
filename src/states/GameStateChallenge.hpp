#ifndef GAMESTATECHALLENGE_HPP
#define GAMESTATECHALLENGE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

class GameStateChallenge : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;

    BoardManager * board;
    ThemeManager theme;

    PauseScreen pause_screen;
    PauseScreen win_screen;
    PauseScreen lose_screen;

    bool paused = false;
    bool completed = false;
    bool failed = false;

    int level;
    int seed;
    int moves;
    int required_matches;
    int width;
    int height;
    SDL_Point position;

    SDL_Point calculatePosition(int width, int height);
    void loadLevel();
public:
    GameStateChallenge(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds);
    ~GameStateChallenge();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATECHALLENGE_HPP