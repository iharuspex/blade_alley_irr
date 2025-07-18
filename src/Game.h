#ifndef GAME_H_
#define GAME_H_

#include <irrlicht/IAnimatedMeshSceneNode.h>
#include <irrlicht/IGUIStaticText.h>
#include <irrlicht/irrlicht.h>
#include <irrlicht/vector3d.h>

#include "EventReceiver.h"

class Game {
public:
    static Game *instance();

    ~Game();

    bool init(const wchar_t *title, int width, int height, bool fullscreen);
    void render();
    void update();
    void handleEvents();
    void clean();
    bool isRunning();

    void quit();

    void printFPS();

private:
    Game();

    static Game *s_pInstance;

    irr::IrrlichtDevice *m_pDevice = nullptr;
    irr::video::IVideoDriver *m_pVideoDriver = nullptr;
    irr::scene::ISceneManager *m_pSceneManager = nullptr;
    irr::gui::IGUIEnvironment *m_pGuiEnvironment = nullptr;

    irr::scene::IAnimatedMesh *mesh;
    irr::scene::IAnimatedMeshSceneNode *node;
    irr::scene::ICameraSceneNode *camera;
    irr::core::vector3df cameraPos;

    int lastFPS = -1;
    irr::u32 thenTime;
    irr::f32 frameDeltaTime;
    irr::gui::IGUIStaticText *fpsGui;


    EventReceiver *m_pEventReceiver;

    // TODO: create logger later
    // irr::ILogger logger;
};

#endif /* GAME_H_ */