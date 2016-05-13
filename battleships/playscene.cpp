#include "playscene.h"

PlayScene PlayScene::playScene;

void PlayScene::renderMyGrid(GameEngine *engine)
{
    for (int i = 0; i < 10; ++i ){
        for ( int j = 0; j < 10; ++j ) {
            SDL_Rect rect;
            rect.w = cellWidth;
            rect.h = cellHeight;
            rect.x = i * cellWidth;
            rect.y = j * cellHeight;
            if ( myGrid.get()[ i ][ j ] == logic::WATER_NOT_SHOT ) {
                renderCell( engine, &rect, 64, 164, 223);
            } else if ( myGrid.get()[ i ][ j ] == logic::WATER_SHOT ) {
                renderCell( engine, &rect, 0, 0, 139 );
            } else if ( myGrid.get()[ i ][ j ] == logic::SHIP_NOT_SHOT ) {
                renderCell( engine, &rect, 130, 82, 1);
            } else if ( myGrid.get()[ i ][ j ] == logic::SHIP_SHOT ) {
                renderCell( engine, &rect, 0, 0, 20);
            } else if ( myGrid.get()[ i ][ j ] == logic::CLICKABLE ) {
                renderCell( engine, &rect, 64, 164, 223);
            } else if ( myGrid.get()[ i ][ j ] == logic::NOT_CLICKABLE ) {
                renderCell( engine, &rect, 116, 127, 151);
            }
        }
    }
    engine->renderer.render();
}

void PlayScene::renderEnemyGrid(GameEngine *engine) {
    for (int i = 0; i < 10; ++i ){
        for ( int j = 0; j < 10; ++j ) {
            SDL_Rect rect;
            rect.w = cellWidth;
            rect.h = cellHeight;
            rect.x = i * cellWidth;
            rect.y = j * cellHeight;
            if ( enemyGrid.get()[ i ][ j ] == logic::WATER_SHOT ) {
                renderCell( engine, &rect, 0, 0, 139 );
            } else if ( enemyGrid.get()[ i ][ j ] == logic::SHIP_SHOT ) {
                renderCell( engine, &rect, 20, 20, 20);
            } else {
                renderCell( engine, &rect, 64, 164, 223);
            }
        }
    }
    engine->renderer.render();
}

std::pair<int, int> PlayScene::getMousePos()
{
    int x;
    int y;
    std::pair< int, int > coordinates;
    SDL_GetMouseState( &x, &y );
    coordinates.first = x / cellWidth;
    coordinates.second = y / cellHeight;
    return coordinates;
}

void PlayScene::renderCell( GameEngine *engine, SDL_Rect *rect, int r, int g, int b )
{
    engine->renderer.setRenderColor( r, g, b, 255 );
    SDL_RenderFillRect( engine->renderer.renderer, rect );
    engine->renderer.setRenderColor( 0, 0, 0, 255 );
    SDL_RenderDrawRect( engine->renderer.renderer, rect );
}

PlayScene::PlayScene()
{

}

void PlayScene::init(GameEngine *engine)
{
    phase = 0;
    clicked = false;
    shipsPlaced = false;
    host = false;
    ready = false;
    layoutReceived = false;
    layoutSent = false;
    getLayout = false;
    getShot = false;
    int width;
    int height;
    SDL_GetRendererOutputSize( engine->renderer.renderer, &width, &height );
    cellWidth = width / 10;
    cellHeight = height / 10;
    myGrid = engine->logic.getClickableMatrix();

  /*  enemyLayout = std::async( std::launch::async,
                              &logic::Logic::getEnemyShipLayout,
                              &engine->logic ); */
    renderMyGrid( engine );
}

void PlayScene::clean()
{

}

void PlayScene::pause()
{

}

void PlayScene::resume(GameEngine *engine)
{

}

void PlayScene::runScene(GameEngine *engine)
{
    if ( phase == 0 ) {
        if ( SDL_WaitEvent( &event ) ) {
            switch ( event.type ) {
            case SDL_QUIT:
                engine->quit();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if ( !shipsPlaced ) {
                    if ( !clicked ) {
                        shipBegin = getMousePos();
                        if ( myGrid.get()[ shipBegin.first ][ shipBegin.second ] == logic::CLICKABLE ) {
                            myGrid = engine->logic.getClickableMatrix( shipBegin.first, shipBegin.second );
                            renderMyGrid( engine );
                            clicked = true;
                        }
                    } else if ( clicked ) {
                        shipEnd = getMousePos();
                        if ( myGrid.get()[ shipEnd.first ][ shipEnd.second ] == logic::CLICKABLE ) {
                            bool correct = engine->logic.insertShip( shipBegin.first,
                                                                     shipBegin.second,
                                                                     shipEnd.first,
                                                                     shipEnd.second );
                            clicked = false;
                            myGrid = engine->logic.getClickableMatrix();
                            shipsPlaced = engine->logic.checkIfAllShipsPlaced();
                            ready = shipsPlaced;
                            renderMyGrid( engine );
                        } else if ( myGrid.get()[ shipEnd.first ][ shipEnd.second ] == logic::NOT_CLICKABLE ) {
                            clicked = false;
                            myGrid = engine->logic.getClickableMatrix();
                            renderMyGrid( engine );
                        }
                    }
                }
                break;
            }
            if ( event.type == engine->logicEventType
                 && event.user.code == HOST ) {
                enemyLayout = std::async( std::launch::async,
                                          &logic::Logic::getEnemyShipLayout,
                                          &engine->logic );
                host = true;
            } else if ( event.type == engine->logicEventType
                        && event.user.code == CONNECT ) {
                host = false;
            } else if ( event.type == engine->logicEventType
                        && event.user.code == GET_LAYOUT ) {
                enemyGrid = enemyLayout.get();
                layoutReceived = true;
            } else if ( event.type == engine->logicEventType
                        && event.user.code == SEND_LAYOUT ) {
                layoutSent = true;
                myGrid = engine->logic.getMyShips();
            }
            if ( ready && host && layoutReceived ) {
                temp = std::async( std::launch::async,
                                   &logic::Logic::sendMyLayout,
                                   &engine->logic );
                ready = false; // so this branch would not be entered twice
            }
            if ( !host && ready ) {
                temp = std::async( std::launch::async,
                                   &logic::Logic::sendMyLayout,
                                   &engine->logic );
                ready = false; // so this branch would not be entered twice
            }
            if ( !host && layoutSent && !layoutReceived && !getLayout) {
                enemyLayout = std::async( std::launch::async,
                                          &logic::Logic::getEnemyShipLayout,
                                          &engine->logic );
                getLayout = true;
            }
            if ( layoutReceived && layoutSent && host ) {
                phase = 2;
                renderMyGrid( engine );
            } else if ( layoutReceived && layoutSent && !host ) {
                phase = 1;
                renderEnemyGrid( engine );
            }
        }
    } else if ( phase == 1 ) {
        if ( SDL_WaitEvent( &event ) ) {
            switch ( event.type ) {
            case SDL_QUIT:
                engine->quit();
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseCoord = getMousePos();
                if ( enemyGrid.get()[ mouseCoord.first ][ mouseCoord.second ] != logic::SHIP_SHOT
                     && enemyGrid.get()[ mouseCoord.first ][ mouseCoord.second ] != logic::WATER_SHOT ) {
                    if ( engine->logic.shootCheck( mouseCoord.first, mouseCoord.second ) ) {
                        enemyGrid.get()[ mouseCoord.first ][ mouseCoord.second ] = logic::SHIP_SHOT;
                        renderEnemyGrid( engine );
                    } else {
                        enemyGrid.get()[ mouseCoord.first ][ mouseCoord.second ] = logic::WATER_SHOT;
                        phase = 2;
                        renderMyGrid( engine );
                    }
                    temp = std::async( std::launch::async,
                                       &logic::Logic::shootSend,
                                       &engine->logic,
                                       mouseCoord.first,
                                       mouseCoord.second );
                }
            }
        }
    } else if ( phase == 2 ) {
        if ( !getShot ) {
            enemyShot =  std::async( std::launch::async,
                                     &logic::Logic::getEnemyShot,
                                     &engine->logic );
            getShot = true;
        }
        if ( SDL_WaitEvent( &event ) ) {
            switch ( event.type ) {
            case SDL_QUIT:
                engine->quit();
                break;
            }
            if ( event.type == engine->logicEventType
                 && event.user.code == ENEMY_SHOT ) {
                shot = enemyShot.get();
                if ( myGrid.get()[ shot.first ][ shot.second ] == logic::SHIP_NOT_SHOT ) {
                    myGrid.get()[ shot.first ][ shot.second ] = logic::SHIP_SHOT;
                    renderMyGrid( engine );
                } else if ( myGrid.get()[ shot.first ][ shot.second ] == logic::WATER_NOT_SHOT ) {
                    myGrid.get()[ shot.first ][ shot.second ] = logic::WATER_SHOT;
                    phase = 1;
                    renderEnemyGrid( engine );
                }
                getShot = false;
            }
        }
    }
}

PlayScene *PlayScene::Instance()
{
    return &playScene;
}
