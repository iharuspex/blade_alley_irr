#include "Game.h"
#include "EventReceiver.h"
#include <irrlicht/EMaterialFlags.h>
#include <irrlicht/IAnimatedMesh.h>
#include <irrlicht/IGUISkin.h>
#include <irrlicht/Keycodes.h>
#include <irrlicht/irrTypes.h>
#include <irrlicht/vector3d.h>

#include <iostream>

using namespace irr;
using namespace core;
using namespace video;

Game *Game::s_pInstance = nullptr;

Game::Game() {}

Game::~Game() = default;

Game *Game::instance()
{
    if (s_pInstance == nullptr) {
        s_pInstance = new Game();
    }

    return s_pInstance;
}

bool Game::init(const wchar_t *title, int width, int height, bool fullscreen) 
{
    m_pEventReceiver = new EventReceiver();

    m_pDevice = irr::createDevice(irr::video::EDT_OPENGL,
                                    core::dimension2d<u32>(width, height), 16,
                                    fullscreen, false, false, m_pEventReceiver);

    if (!m_pDevice) {
        return false;
    }

    m_pDevice->setWindowCaption(title);

    m_pVideoDriver = m_pDevice->getVideoDriver();
    m_pSceneManager = m_pDevice->getSceneManager();
    m_pGuiEnvironment = m_pDevice->getGUIEnvironment();

    mesh = m_pSceneManager->getMesh("../assets/heli.X");
    node = m_pSceneManager->addAnimatedMeshSceneNode(mesh);

    if (node) {
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMaterialFlag(EMF_GOURAUD_SHADING, true);
        node->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
        node->setScale(vector3df(.5, .5, .5));
    }

    camera = m_pSceneManager->addCameraSceneNode(0, vector3df(60, 40, -40), vector3df(0, 5, 0));
    cameraPos = camera->getPosition();

    irr::gui::IGUISkin* skin = m_pGuiEnvironment->getSkin();
    irr::gui::IGUIFont* font = m_pGuiEnvironment->getFont("../assets/bigfont.png");
    skin->setColor(irr::gui::EGDC_BUTTON_TEXT, video::SColor(255, 255, 255, 0));
    if (font) {
        skin->setFont(font);
    } else {
        std::cout << "Oops\n";
    }
        

    fpsGui = m_pGuiEnvironment->addStaticText(L"FF", 
        rect<s32>(10, 0, 100, 30), true);

    thenTime = m_pDevice->getTimer()->getTime();

    return true;
}

void Game::render()
{
    m_pVideoDriver->beginScene(true, true, video::SColor(255, 100, 101, 140));

    m_pSceneManager->drawAll();
    m_pGuiEnvironment->drawAll();

    m_pVideoDriver->endScene();
}

void Game::printFPS()
{
    int fps = m_pVideoDriver->getFPS();

    if (lastFPS != fps) {
        core::stringw tmp(L"");
        tmp += fps;

        fpsGui->setText(tmp.c_str());
        lastFPS = fps;
    }
}

void Game::update()
{
    // fps
    irr::u32 now = m_pDevice->getTimer()->getTime();
    frameDeltaTime = (irr::f32)(now - thenTime) / 1000.f;
    thenTime = now;

    printFPS();
}

const f32 MOVEMENT_SPEED = 20.f;

void Game::handleEvents()
{
    if (m_pEventReceiver->IsKeyDown(irr::KEY_KEY_W)) {
        cameraPos.Y += MOVEMENT_SPEED * frameDeltaTime;
    } else if (m_pEventReceiver->IsKeyDown(irr::KEY_KEY_S)){
        cameraPos.Y -= MOVEMENT_SPEED * frameDeltaTime;
    } else if (m_pEventReceiver->IsKeyDown(irr::KEY_KEY_A)){
        cameraPos.X -= MOVEMENT_SPEED * frameDeltaTime;
    } else if (m_pEventReceiver->IsKeyDown(irr::KEY_KEY_D)){
        cameraPos.X += MOVEMENT_SPEED * frameDeltaTime;
    }

    camera->setPosition(cameraPos);
}

void Game::clean()
{
    m_pDevice->drop();
}

bool Game::isRunning()
{
    return m_pDevice->run();
}

void Game::quit()
{

}