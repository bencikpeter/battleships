//
//  main.cpp
//  battleships
//
//  Created by Peter Bencik on 26.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "logicevent.h"
#include "gameengine.h"
#include "menuscene.h"

int main ( )
{
    GameEngine engine("battleships", 1020, 760);

    engine.changeScene( MenuScene::Instance());

    while ( engine.isRunning() )
    {
        engine.runScene();
    }

    return 0;
}
