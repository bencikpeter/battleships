//
//  main.cpp
//  battleships
//
//  Created by Peter Bencik on 26.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#include <iostream>
#include "SDL.h"
#include "logicevent.h"
#include <future>

int func() {
    LogicEvent event;
    return event.getEventType();
}

int main(){
    using std::cout;
    using std::endl;
    using std::cerr;

    const int width = 400;
    const int height = 200;

    if ( SDL_Init(SDL_INIT_EVERYTHING) ) {
        cout << "Failed to initialize SDL : " << SDL_GetError() << endl;
        return 1;
    }

    const Uint32 logicEventType = SDL_RegisterEvents(1);

    if ( logicEventType != (Uint32)-1 ) {
        LogicEvent::setType( logicEventType );
    } else {
        cerr << "Could not initialize business logic event : " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window;
    window = SDL_CreateWindow( "Battleships !",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width, // window width
                               height, // window height
                               SDL_WINDOW_RESIZABLE );

    if ( window == nullptr ) {
        cout << "There was error initializing the window : " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

    if ( renderer == nullptr ) {
        cout << "Error initializing renderer : " << SDL_GetError() << endl;
        return 1;
    }

    SDL_RenderSetLogicalSize( renderer, width, height );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );


    SDL_Surface* screen = SDL_GetWindowSurface( window );

    Uint32 white = SDL_MapRGB( screen->format, 255, 255, 255 );

    SDL_FillRect( screen, nullptr, white );

    SDL_UpdateWindowSurface( window );

    SDL_Event event;
    bool running = true;

    std::future<int> x;

    while ( running ) {
        while( SDL_WaitEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {
                running = false;
                break;
            }
            if ( event.type == logicEventType ) {
                cout << "Heureka, got message from logic !" << endl;
                cout << x.get() << endl;
                SDL_RenderClear( renderer );
                SDL_RenderPresent( renderer );
            }
            if ( event.type == SDL_KEYDOWN ) {
                switch ( event.key.keysym.scancode ) {
                case SDL_SCANCODE_KP_0:
                    x = std::async( std::launch::async, func );
                    break;
                }
            }
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
}
