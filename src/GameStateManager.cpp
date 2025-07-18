#include "GameStateManager.h"

void GameStateManager::update()
{
    if (!m_gameStates.empty()) {
        m_gameStates.back()->update();
    }
}

void GameStateManager::render()
{
    if (!m_gameStates.empty()) {
        m_gameStates.back()->render();
    }
}

void GameStateManager::pushState(GameState *pState)
{
    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateManager::changeState(GameState *pState)
{
    if (!m_gameStates.empty()) {
        if (m_gameStates.back()->getStateID() == pState->getStateID()) {
            return;
        }

        popState();
    }

    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateManager::popState()
{
    if (!m_gameStates.empty()) {
        if (m_gameStates.back()->onExit()) {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }
}