#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <string>

enum class GameStateType 
{
    MENU,
    PLAYING,
    PAUSED,
};

class GameState 
{
public:
    virtual ~GameState() = 0;

    virtual void update() = 0;
    virtual void render() = 0;
    virtual void handleInput() = 0;

    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    virtual std::string getStateID() const = 0;
};

#endif /* GAMESTATE_H_ */