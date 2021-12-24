#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Shell.hpp"

class Board {

private:
    std::vector<std::vector<Shell>> shells;

    bool isWithinBounds(SDL_Point point);
    std::vector<Shell> getMatches(std::vector<std::vector<Shell>> shells, std::vector<SDL_Point> * matchedGems);
    std::vector<std::vector<Shell>> getShellsCopy();

    int getCount(Shell shell);

    void dropNewShell(int x);
    bool isFalling(int x);
    bool shellsMatch(std::vector<std::vector<Shell>> shells, SDL_Point p1, SDL_Point p2, SDL_Point p3);
public:
    Board(int width, int height, int seed);
    ~Board();

    std::vector<Shell> match();

    std::vector<SDL_Point> dropShells();

    bool hasEmpty();

    bool swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<Shell>> getShells(){return shells;};
    std::vector<std::vector<Shell>> getShellsAfterSwap(std::vector<std::vector<Shell>> shells, SDL_Point p1, SDL_Point p2);
};

#endif // BOARD_HPP
