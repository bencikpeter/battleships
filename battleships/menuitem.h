#ifndef MENUITEM_H
#define MENUITEM_H

#include "SDL.h"
#include <string>
#include "SDL_ttf.h"
#include "gameengine.h"

class MenuItem
{
public:
    std::string text;
    TTF_Font* font;
    SDL_Texture* notSelected;
    SDL_Texture* selected;
    SDL_Rect rect;

    bool isSelected() {
        int x, y;
        SDL_GetMouseState( &x, &y );
        return x >= rect.x && x <= rect.x + rect.w
                && y >= rect.y && y <= rect.y + rect.h;
    }

    void computeDimmensions() {
        SDL_QueryTexture( notSelected, nullptr, nullptr, &rect.w, &rect.h );
    }

friend GameEngine;


public:
MenuItem(const char* text , TTF_Font *font ):text( text ),
    font( font ),
    notSelected( nullptr),
    selected( nullptr ),
    rect( { 0, 0, 0, 0 } ) {}
};

#endif // MENUITEM_H
