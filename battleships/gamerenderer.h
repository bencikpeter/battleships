#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "SDL.h"

class GameEngine;

/**
 * @brief The GameRenderer class renders to the monitor
 */
class GameRenderer
{
private:
    SDL_Window *window;

public:
    SDL_Renderer *renderer;

    friend GameEngine;
    GameRenderer();
    /**
     * @brief init initialize SDL_Renderer
     * @param window
     */

    void init( SDL_Window *window );

    /**
     * @brief close closes SDL_Renderer
     */
    void close();

    /**
     * @brief setRenderColor set renderer color
     * @param red red channel
     * @param green green channel
     * @param blue blue channel
     * @param alfa alfa channel
     */
    void setRenderColor( Uint32 red, Uint32 green, Uint32 blue, Uint32 alfa );

    /**
     * @brief clear clears the screen to the color set by setRenderColor
     */
    void clear();

    /**
     * @brief render apply all rendered changes i.e. display them to the monitor
     */
    void render();

    void renderCopy(SDL_Texture *texture, SDL_Rect &rect);

    void computeRect(SDL_Texture *texture, int offset , SDL_Rect &rect);
};

#endif // GAMERENDERER_H
