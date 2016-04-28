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
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    SDLNet_Quit();
    SDL_Quit();
}
