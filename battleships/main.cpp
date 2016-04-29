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
    SDL_Init(SDL_INIT_EVERYTHING);

    const Uint32 logicEventType = SDL_RegisterEvents(1);

    if ( logicEventType != (Uint32)-1 ) {
        LogicEvent::setType( logicEventType );
    } else {
        cerr << "Could not initialize business logic event";
        exit(1);
    }

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
            }
            x = std::async( std::launch::async, func );
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
}
