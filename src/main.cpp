#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "structures.h"
using namespace std; 



// hardcode map instead of using sprite sheet
vector<vector<int> > createMap() {
    vector<vector<int> > map = {
        {2,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,1,0,0,0,0,2,2,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,0,0,2,2,2,2,0,0,2,2,2,2,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,2,2,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
        {0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
        {0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    return map;
}

const vector<vector<int> > MAP = createMap();



// Function to create a colored rectangle (simulating pixel art)
SDL_Surface* createTileTexture(int r, int g, int b) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    return surface;
}

bool isColliding(float x, float y, const vector<vector<int> >& map) {
    int tileX = static_cast<int>(x) / TILE_SIZE;
    int tileY = static_cast<int>(y) / TILE_SIZE;
    
    if (tileY >= 0 && tileY < map.size() && tileX >= 0 && tileX < map[0].size()) {
        return map[tileY][tileX] == 1 || map[tileY][tileX] == 2; // Water and trees are solid
    }
    return true;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("os",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    // Create tile textures
    SDL_Surface* grassTile = createTileTexture(124, 252, 0);    // Light green
    SDL_Surface* waterTile = createTileTexture(0, 119, 190);    // Blue
    SDL_Surface* treeTile = createTileTexture(34, 139, 34);     // Dark green
    SDL_Surface* playerTile = createTileTexture(255, 192, 203); // Pink
    
    SDL_Texture* grassTexture = SDL_CreateTextureFromSurface(renderer, grassTile);
    SDL_Texture* waterTexture = SDL_CreateTextureFromSurface(renderer, waterTile);
    SDL_Texture* treeTexture = SDL_CreateTextureFromSurface(renderer, treeTile);
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerTile);
    
    SDL_FreeSurface(grassTile);
    SDL_FreeSurface(waterTile);
    SDL_FreeSurface(treeTile);
    SDL_FreeSurface(playerTile);
    
    // Initialize player
    Player player = {
        WINDOW_WIDTH / 2.0f,
        WINDOW_HEIGHT / 2.0f,
        0,
        false,
        {0, 0, TILE_SIZE, TILE_SIZE}
    };
    
    // Initialize camera
    Camera camera = {0, 0};
    
    bool quit = false;
    SDL_Event event;
    
    // Game loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Handle keyboard input
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        float newX = player.x;
        float newY = player.y;
        
        if (currentKeyStates[SDL_SCANCODE_W]) newY -= PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_S]) newY += PLAYER_SPEED;
        if (currentKeyStates[SDL_SCANCODE_A]) {
            newX -= PLAYER_SPEED;
            player.facingLeft = true;
        }
        if (currentKeyStates[SDL_SCANCODE_D]) {
            newX += PLAYER_SPEED;
            player.facingLeft = false;
        }
        if ((currentKeyStates[SDL_SCANCODE_D] && currentKeyStates[SDL_SCANCODE_W]) || 
            (currentKeyStates[SDL_SCANCODE_A] && currentKeyStates[SDL_SCANCODE_W]) ||
            (currentKeyStates[SDL_SCANCODE_A] && currentKeyStates[SDL_SCANCODE_S]) ||
            (currentKeyStates[SDL_SCANCODE_S] && currentKeyStates[SDL_SCANCODE_D])){

            }
        
        // Check collision before updating position
        if (!isColliding(newX, player.y, MAP)) player.x = newX;
        if (!isColliding(player.x, newY, MAP)) player.y = newY;
        
        // Update camera to follow player
        camera.x = static_cast<int>(player.x - WINDOW_WIDTH / 2);
        camera.y = static_cast<int>(player.y - WINDOW_HEIGHT / 2);
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Render map
        for (size_t y = 0; y < MAP.size(); ++y) {
            for (size_t x = 0; x < MAP[y].size(); ++x) {
                SDL_Rect destRect = {
                    static_cast<int>(x * TILE_SIZE) - camera.x,
                    static_cast<int>(y * TILE_SIZE) - camera.y,
                    TILE_SIZE,
                    TILE_SIZE
                };
                
                // Only render tiles that are visible
                if (destRect.x + TILE_SIZE >= 0 && destRect.x <= WINDOW_WIDTH &&
                    destRect.y + TILE_SIZE >= 0 && destRect.y <= WINDOW_HEIGHT) {
                    switch (MAP[y][x]) {
                        case 0:
                            SDL_RenderCopy(renderer, grassTexture, NULL, &destRect);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, waterTexture, NULL, &destRect);
                            break;
                        case 2:
                            SDL_RenderCopy(renderer, treeTexture, NULL, &destRect);
                            break;
                    }
                }
            }
        }
        
        // Render player
        SDL_Rect playerRect = {
            static_cast<int>(player.x) - camera.x,
            static_cast<int>(player.y) - camera.y,
            TILE_SIZE,
            TILE_SIZE
        };
        SDL_RenderCopyEx(renderer, playerTexture, NULL, &playerRect,
            0, NULL, player.facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16); // Cap at ~60 FPS
    }
    
    // Cleanup
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
    SDL_DestroyTexture(treeTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}