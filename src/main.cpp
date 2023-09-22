#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv){
    IrrlichtDevice *device = createDevice(EDT_SOFTWARE, dimension2du(640, 480), 16, false, false, false, 0);

    device->setWindowCaption(L"Blade Alley Irr");

    IVideoDriver *driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();

    IAnimatedMesh* mesh = smgr->getMesh("../model/susanna.obj");
    if (!mesh) {
        device->drop();
        return 1;
    }
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setScale(vector3df(8, 8, 8));
    }

    smgr->addCameraSceneNode(nullptr, vector3df(0, 30, -40), vector3df(0, 5, 0));

    while (device->run()) {
        driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
        node->setRotation(node->getRotation() + vector3df(0, 0.5, 0));
        driver->endScene();
    }

    device->drop();

    return 0;
}
