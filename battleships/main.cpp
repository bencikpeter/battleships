//
//  main.cpp
//  battleships
//
//  Created by Peter Bencik on 26.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#include <iostream>
#include "SDL.h"
#include "SDL_net.h"

int main(){
    using std::cout;
    using std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window;
    window = SDL_CreateWindow( "Battleships !",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               400, // window width
                               200, // window height
                               SDL_WINDOW_RESIZABLE );

    if ( window == nullptr ) {
        cout << "There was error initializing the window !" << endl;
    }

    SDL_Surface* screen = SDL_GetWindowSurface( window );

    Uint32 white = SDL_MapRGB( screen->format, 255, 255, 255 );

    SDL_FillRect( screen, nullptr, white );

    SDL_UpdateWindowSurface( window );

    SDL_Event event;
    bool running = true;

    while ( running ) {
        while( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {
                running = false;
                break;
            }
        }

    }

    SDL_DestroyWindow( window );

    SDLNet_Init();

    SDLNet_Quit();
    SDL_Quit();
}
