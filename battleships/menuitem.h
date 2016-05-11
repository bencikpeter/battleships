#ifndef MENUITEM_H
#define MENUITEM_H

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include "gameengine.h"

class MenuItem
{
public:
    std::string text;
    TTF_Font* font;
    SDL_Texture* notSelected;
    SDL_Texture* selected;
    SDL_Rect rect;

    bool isSelected();

    void computeDimmensions();

friend GameEngine;


public:
    MenuItem(const char* text , TTF_Font *font );
};

#endif // MENUITEM_H
