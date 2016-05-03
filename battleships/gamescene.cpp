#include "gamescene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::changeState(GameEngine *engine, GameScene *scene)
{
    engine->changeScene( scene );
}
