#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "gamescene.h"
#include "gamerenderer.h"
#include "logicevent.h"
#include "business_logic_backend.h"
#include "SDL.h"
#include <stack>

/**
 * @brief The GameEngine class takes care of game scenes
 */
class GameEngine
{
public:
    GameEngine(const char *title, int width, int height);

    ~GameEngine();

    /**
     * @brief changeScene changes the scene on the top of the stack
     * @param scene
     */
    void changeScene( GameScene* scene );

    /**
     * @brief pushScene pushes the scene to the stack
     * @param scene
     */
    void pushScene( GameScene* scene );

    /**
     * @brief popScene pops scene from the stack
     */
    void popScene();

    /**
     * @brief runScene run scene on the top of the stack
     */
    void runScene();

    /**
     * @brief quit prevent game loop from another iteration
     */
    void quit();

    /**
     * @brief renderer renders scene to the monitor
     */
    GameRenderer renderer;

    /**
     * @brief logic makes actual logic ingame
     */
    logic::Logic logic;

    /**
     * @brief getLogicEventType
     * @return type of event caused by logic
     */
    Uint32 getLogicEventType() const;

private:
    SDL_Window* window;
    std::stack<GameScene*> scenes;
    bool running;
    Uint32 logicEventType;
};

#endif // GAMEENGINE_H
