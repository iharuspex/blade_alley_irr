#ifndef GAMESTATECONTROLLER_H_
#define GAMESTATECONTROLLER_H_

#include <vector>

#include "GameState.h"

class GameStateManager
{
public:
    void update();
    void render();

    void pushState(GameState *pState);
    void changeState(GameState *pState);
    void popState();

private:
    std::vector<GameState *> m_gameStates;
};

#endif /* GAMESTATECONTROLLER_H_ */