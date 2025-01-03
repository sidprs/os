#include <SDL2/SDL.h>
#include <vector>
#include <string>


const int TILE_SIZE = 32;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SPEED = 4;

struct Player {
    float x, y;
    int frame;
    bool facingLeft;
    SDL_Rect currentClip;
};

struct Camera {
    int x, y;
};


#ifndef STRUCTURE_H
#define STRUCTURE_H


class structures {
    public:
        SDL_Surface* createTileTexture(int r, int g, int b);
        bool isColliding(float x, float y, const std::vector<std::vector<int> >& map);
        Player createPlayer();
        Camera createCam();




};

#endif
