#include "menuitem.h"

bool MenuItem::isSelected() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    return x >= rect.x && x <= rect.x + rect.w
            && y >= rect.y && y <= rect.y + rect.h;
}

void MenuItem::computeDimmensions() {
    SDL_QueryTexture( notSelected, nullptr, nullptr, &rect.w, &rect.h );
}

MenuItem::MenuItem( const char *text, TTF_Font *font ):text( text ),
                                                       font( font ),
                                                       notSelected( nullptr),
                                                       selected( nullptr ),
                                                       rect( { 0, 0, 0, 0 } ) {}

