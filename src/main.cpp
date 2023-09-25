#include <irrlicht/irrlicht.h>
// #include "driverChoice.h"

#include "irrKlang.h"
#include <time.h>
// #include <windows.h>
#include <unistd.h>
#include <math.h>
#include <irrlicht/IVideoDriver.h>
/*

#include <CEffectPostProc.h>
#include <XEffects.h>
#include <CLensFlarePostProc.h>
#include <CRendererPostProc.h>
#include <CScreenQuad.h>
#include <CShaderCallbacks.h>
#include <CShaderPre.h>
#include <CSplitPostProc.h>
#include <CTransitionPostProc.h>
#include <CWaterPostProc.h>
#include <EffectCB.h>
#include <EffectHandler.h>
#include <EffectShaders.h>
#include <IPostProc.h>
#include <shaders.h>*/
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;

//ISoundEngine* engine = createIrrKlangDevice();


class MyEventReceiver : public IEventReceiver {
public:
    // наш собственный обработчик событий
    virtual bool OnEvent(const SEvent &event) {
        // просто запоминаем состояние любой клавиши - нажата/отжата
        if (event.EventType == EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        return false;
    }

    // метод возвращающий состояние для запрошенной клавиши
    virtual bool IsKeyDown(EKEY_CODE keyCode) const {
        return KeyIsDown[keyCode];
    }

    //конструктор, в цикле сбрасываем статус для всех клавиш
    MyEventReceiver() {
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // массив для хранения статусов клавиш
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


MyEventReceiver receiver;
IrrlichtDevice *device =
        createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 16,
                     false, true, true, &receiver);
IVideoDriver *driver = device->getVideoDriver();
ISceneManager *smgr = device->getSceneManager();
IAnimatedMesh *mesh = 0;
IAnimatedMesh *sky = 0;
IAnimatedMesh *colona = 0;
IAnimatedMesh *tarelka = 0;
IAnimatedMesh *nlo = 0;
IAnimatedMeshSceneNode *anode = 0;
IAnimatedMeshSceneNode *colonod[30];
IAnimatedMeshSceneNode *skynod;
IAnimatedMeshSceneNode *enemy[10];
ISceneNode *node1 = 0;
ICameraSceneNode *cam;
IBillboardSceneNode *bill1;
IBillboardSceneNode *bill2;
IParticleSystemSceneNode *ps;
IParticleSystemSceneNode *ps2;
ISceneNode *cube[300];
ISceneNode *roket[2];

ITexture *image;
ITexture *image2;
f32 fdt;

int deadtime;
int bum;

const float speed = 150;
float friction = 0.93f;
float dx = 0.0f, dy = 0.0f;
int attack = 0;
float sme = 0;
float posx = 330, posy = -150;
float newposx = 0, newposy = 0, kren = 0, camx = 500 - posx / 2, camy = -posy / 2, camz = 300;
float speedrun = .5;

bool start = false, camstat = false;
bool fire;
u32 then = device->getTimer()->getTime();
int en_num;
int en[10];
int destr, live;


int en_num_dr;

void displ(int ene, int li);

void game();

void mid();

void crush();

void ini() {
    en_num_dr = 0;
    camx = 500 - posx / 2;
    camz = 300;
    camy = -posy / 2;
    live = 3;
    destr = 15;
    deadtime = 0;

    bill1->setVisible(true);
    bill2->setVisible(true);
    bum = 0;
    fire = false;
    en_num = 0;
    ps2->setPosition(vector3df(0, -1000, 0));
    cam->setPosition(vector3df(350, camy, camz));
    cam->setTarget(vector3df(350, posy - 500, 380));
}


int main() {
    device->setWindowCaption(L"Blade Alley Irr!");


    device->getCursorControl()->setVisible(false);
    device->getFileSystem()->addZipFileArchive("../assets/res.asset");
// engine->play2D("../assets/1.mp3", false);

    mesh = smgr->getMesh("../assets/heli.X");
    colona = smgr->getMesh("../assets/colona.X");
    sky = smgr->getMesh("../assets/skybox.X");
    tarelka = smgr->getMesh("../assets/tarelka.X");
    nlo = smgr->getMesh("../assets/nlo.X");

    driver->setFog(SColor(240, 180, 0, 0), EFT_FOG_LINEAR, 300, 4500, .0001f, true, false);


    anode = smgr->addAnimatedMeshSceneNode(mesh);


    anode->addShadowVolumeSceneNode();
    anode->setPosition(vector3df(350, 0, 400));
    //anode->setMaterialFlag(EMF_LIGHTING, true);
    anode->setMaterialFlag(EMF_GOURAUD_SHADING, true);
    anode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);

    // anode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    //anode->setRotation(vector3df(0,1,0));
    anode->setScale(vector3df(.5, .5, .5));
    //  anode->setScale(vector3df(.5,1,.5));
    //  anode->setAnimationSpeed(15);
    skynod = smgr->addAnimatedMeshSceneNode(sky);
    skynod->setMaterialFlag(EMF_LIGHTING, false);
    skynod->setMaterialTexture(0, driver->getTexture(
            "../assets/back.jpeg"));
    skynod->setScale(vector3df(80, 40, 40));
    skynod->setRotation(vector3df(0, 90, 0));
    skynod->setPosition(vector3df(200, 0, -1500));
    skynod->setMaterialFlag(EMF_FOG_ENABLE, true);

    int l = 0;
    for (int i = 0; i < 15; i++) {
        for (int f = -2500; f < 2500; f += 300) {
            cube[l] = smgr->addCubeSceneNode();
            cube[l]->setScale(vector3df(30.0f, 0.0f, 30.0f));
            cube[l]->setPosition(vector3df(f, -320, i * 300));
            cube[l]->setMaterialFlag(EMF_LIGHTING, false);

            /*cube[l]->setMaterialTexture(0, driver->getTexture(
                    "../assets/lava.jpg"));*/
            cube[l]->setMaterialTexture(0, driver->getTexture("../assets/lava.jpg"));
            cube[l]->setMaterialFlag(EMF_FOG_ENABLE, true);

            l++;
        }

        colonod[i] = smgr->addAnimatedMeshSceneNode(colona);
        if ((i % 2) == true)
            colonod[i]->setMaterialTexture(0, driver->getTexture(
                    "../assets/colotex2.png"));
        colonod[i]->setScale(vector3df(.4, .5, .4));
        colonod[i]->setPosition(vector3df(0, 0, i * 300));
        colonod[i]->setRotation(vector3df(0, 90, 0));
        colonod[i]->setMaterialFlag(EMF_FOG_ENABLE, true);
    }
    for (int i = 15; i < 30; i++) {
        colonod[i] = smgr->addAnimatedMeshSceneNode(colona);
        if ((i % 2) == true)
            colonod[i]->setMaterialTexture(0, driver->getTexture(
                    "../assets/colotex2.png"));
        colonod[i]->setScale(vector3df(.4, .5, .4));
        colonod[i]->setPosition(vector3df(800, 0, (i - 15) * 300));
        colonod[i]->setRotation(vector3df(0, 90, 0));
        colonod[i]->setMaterialFlag(EMF_FOG_ENABLE, true);
    }
    // add a first person shooter style user controlled camera


    node1 = smgr->addLightSceneNode(0, vector3df(400, 400, 900), SColorf(1.0f, 1.0f, 1.0f, 0.0f), 5000.0f);
    cam = smgr->addCameraSceneNode(0, vector3df(350, 0, 00), vector3df(350, 0, 1000));

//ICameraSceneNode* cam=smgr->addCameraSceneNodeFPS();

//cam->setFOV(90);
//cam->setPosition(vector3df(350,0,200));
//ICameraSceneNode* cam=smgr->addCameraSceneNode(0, vector3df(300,-200,200), vector3df(300,-150,400));
    smgr->getActiveCamera()->setFarValue(10000);

    ps = smgr->addParticleSystemSceneNode(false);
    ps2 = smgr->addParticleSystemSceneNode(false);


    IParticleEmitter *em = ps->createBoxEmitter(
            aabbox3d<f32>(-5, -5, 0, 5, 5, -5), // размер эмиттера(куба)
            vector3df(0.0f, 0.0f, -1.5f),   // начальное направление
            2000, 2500,                             // частота испускания (мин,макс)
            SColor(0, 255, 255, 255),       // самый темный цвет
            SColor(0, 255, 255, 255),       // самый яркий цвет
            100, 200, 0,                        // време жизни (мин,макс), угол
            dimension2df(5.f, 5.f),         // минимальный размер частиц
            dimension2df(15.f, 15.f));        // максимальный размер частиц

    ps->setEmitter(em); // отдаем эмиттер системе
    // а лично нам он не нужен

    IParticleAffector *paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // отдаем аффектор системе и дропаем, т.к. нам он не нужен


    ps->setScale(vector3df(2, 2, 2));
    ps->setMaterialFlag(EMF_LIGHTING, false);
    ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("../assets/fire.bmp"));
    ps->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);


    em = ps2->createBoxEmitter(
            aabbox3d<f32>(-50, -10, 0, 50, 10, 10), // размер эмиттера(куба)
            vector3df(0.0f, 0.0f, -1.0f),   // начальное направление
            1000, 1050,                             // частота испускания (мин,макс)
            SColor(0, 255, 255, 255),       // самый темный цвет
            SColor(0, 255, 255, 255),       // самый яркий цвет
            800, 850, 0,                        // време жизни (мин,макс), угол
            dimension2df(20.f, 20.f),         // минимальный размер частиц
            dimension2df(20.f, 20.f));        // максимальный размер частиц

    ps2->setEmitter(em); // отдаем эмиттер системе
    em->drop(); // а лично нам он не нужен

    paf = ps2->createFadeOutParticleAffector();

    ps2->addAffector(paf); // отдаем аффектор системе и дропаем, т.к. нам он не нужен
    paf->drop();


    ps2->setScale(vector3df(2, 2, 2));
    ps2->setMaterialFlag(EMF_LIGHTING, false);
    ps2->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
    ps2->setMaterialTexture(0, driver->getTexture("../assets/fire2.bmp"));
    ps2->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);


    /*
    IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2d( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
    CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2d( 1024, 512 ), PP_BLUR, 0.01 );
    //зададим уровень качества эффекта
    ppBlur->setQuality( PPQ_FAST );
    */
    // draw everything
    bill1 = smgr->addBillboardSceneNode();
    bill1->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    bill1->setMaterialTexture(0, driver->getTexture(
            "../assets/1.png"));

    bill1->setSize(dimension2d<f32>(150.0f, 100.0f));
    bill1->setPosition(vector3df(250, -120, 300));
    bill2 = smgr->addBillboardSceneNode();
    bill2->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    bill2->setMaterialTexture(0, driver->getTexture("../assets/2.png"));
    bill2->setSize(dimension2d<f32>(150.0f, 100.0f));
    bill2->setPosition(vector3df(420, -120, 300));


    roket[0] = smgr->addCubeSceneNode();
    roket[0]->setVisible(false);
    roket[0]->setScale(vector3df(1.0f, 1.0f, 5.0f));
    roket[1] = smgr->addCubeSceneNode();
    roket[1]->setVisible(false);
    roket[1]->setScale(vector3df(1.0f, 1.0f, 5.0f));


    ini();
    image = driver->getTexture("../assets/ufo.png");
    image2 = driver->getTexture("../assets/live.png");


    while (device->run() && driver) {
        game();

        driver->beginScene(true, true, SColor(255, 120, 0, 0));
        smgr->drawAll();

//driver->enableMaterial2D(true);
        if (start == true)displ(destr, live);

        if (deadtime > 0)crush();
        else {
            anode->setVisible(true);
        }


        usleep(10000);


//device->getGUIEnvironment()->drawAll();


        driver->endScene();


    }


    // delete device
    device->drop();
    return 0;
}


void displ(int ene, int li) {
    driver->draw2DRectangle(SColor(100, 255, 255, 255),
                            rect<s32>(10, 565, 470, 590));
    driver->draw2DRectangle(SColor(100, 255, 255, 255),
                            rect<s32>(10, 530, 150, 560));
    for (int i = 0; i < ene; i++) {
        driver->draw2DImage(image, position2di(15 + i * 30, 565), recti(0, 0, 24, 24), 0, SColor(255, 255, 255, 255),
                            true);
    }
    for (int i = 0; i < li; i++) {
        driver->draw2DImage(image2, position2di(15 + i * 44, 535), recti(0, 0, 39, 24), 0, SColor(255, 255, 255, 255),
                            true);
    }
}

void game() {

    const u32 now = device->getTimer()->getTime();
    fdt = (f32) (now - then) / 1000.f;
    then = now;
    vector3df anodePosition = anode->getPosition();


    ps->setPosition(anodePosition + vector3df(0, 10, -50));
    if (start == false && camstat == false) {

        speedrun = 80 * fdt;
        cam->setPosition(vector3df(350, camy, camz));
        cam->setTarget(vector3df(350, posy - 500, 380));
        if (receiver.IsKeyDown(KEY_SPACE)) {
            camstat = true;
        }

    } else if (start == false) {

        if (camz > -200)camz -= 800 * fdt;
        cam->setPosition(vector3df(350, -posy / 2, camz));
        vector3df tar = cam->getTarget();
//if(tar.X <350)tar.X++;
        if (tar.Y < 500)tar.Y += (400 * fdt);
        if (tar.Y > 500)tar.Y -= (400 * fdt);
        if (tar.Z < 1000)tar.Z += (400 * fdt);
        cam->setTarget(tar);
        if (tar.Z >= 1000) {
            start = true;
            camstat = false;
            bill1->setVisible(false);
            bill2->setVisible(false);
        }
    } else {


        if (receiver.IsKeyDown(KEY_SPACE) && fire == false) {

            roket[0]->setPosition(anodePosition + vector3df(-35, 10, 100));
            roket[1]->setPosition(anodePosition + vector3df(35, 10, 100));
            roket[0]->setVisible(true);
            roket[1]->setVisible(true);
            fire = true;
        }

        if (fire == true) {
            vector3df po = roket[0]->getPosition();
            roket[0]->setPosition(po + vector3df(0, 0, 3000 * fdt));
            po = roket[1]->getPosition();
            roket[1]->setPosition(po + vector3df(0, 0, 3000 * fdt));

            if (po.Z > 4000) {
                roket[0]->setVisible(false);
                roket[1]->setVisible(false);
                fire = false;
            }

            for (int i = 0; i < en_num_dr; i++) {
                vector3df enpo = enemy[i]->getPosition();
                if (po.X > enpo.X - 100 && po.X < enpo.X + 100 &&
                    po.Y > enpo.Y - 100 && po.Y < enpo.Y + 100 &&
                    po.Z > enpo.Z - 100 && po.Z < enpo.Z + 100) {
                    enemy[i]->setPosition(vector3df(0, -1000, 0));
                    roket[0]->setVisible(false);
                    roket[1]->setVisible(false);
                    fire = false;

                    if (en[i] == 0)destr--;

                    ps2->setPosition(enpo);
                    bum = 800 * fdt;
                }
            }
        }

        if (bum > 0) {

            vector3df rot = ps2->getRotation();
            ps2->setRotation(rot + vector3df(0, 0, 800 * fdt));
            bum -= 1 * fdt;
            cam->setPosition(
                    vector3df(500 - posx / 2 + rand() % 20 - rand() % 20, -posy / 2 + rand() % 20 - rand() % 20, -200));
        } else {
            if (deadtime <= 0) {
                ps2->setPosition(vector3df(0, -1000, 0));
                cam->setPosition(vector3df(500 - posx / 2, -posy / 2, -200));
            }
        }

        attack += 1000 * fdt;
        if (attack > 700 && en_num < 9) {
            en[en_num] = rand() % 2;
            if (en[en_num] == 0) {
                enemy[en_num] = smgr->addAnimatedMeshSceneNode(tarelka);
                enemy[en_num]->setScale(vector3df(.35, .45, .35));
            }
            if (en[en_num] == 1) {
                enemy[en_num] = smgr->addAnimatedMeshSceneNode(nlo);
                enemy[en_num]->setScale(vector3df(.2, .2, .2));
            }

            if (en[en_num] == 0)
                enemy[en_num]->setPosition(vector3df(150 + rand() % 450, -200 + rand() % 400, 5000 - rand() % 200));
            if (en[en_num] == 1)
                enemy[en_num]->setPosition(vector3df(150 + rand() % 450, rand() % 400 - 150, 5000 - rand() % 200));

            enemy[en_num]->setMaterialFlag(EMF_FOG_ENABLE, true);


            en_num++;
            if (en_num_dr < 9) en_num_dr++;
            attack = 0;
        }
        if (en_num >= 9)en_num = 0;

        if (attack > 10) {
            for (int i = 0; i < en_num_dr; i++) {

                vector3df erot = enemy[i]->getRotation();
                if (en[i] == 0)enemy[i]->setRotation(vector3df(erot.X, erot.Y += 300 * fdt, erot.Z));
                vector3df enpos = enemy[i]->getPosition();
                if (en[i] == 0)enpos.X += 2 * cos(enpos.Z / 160);
                if (en[i] == 1)enpos.Y += 4 * cos(enpos.Z / 400);
                enemy[i]->setPosition(vector3df(enpos.X, enpos.Y, enpos.Z -= 2800 * fdt));
            }

        }


        if (speedrun < (1900 * fdt))speedrun += 10 * fdt;
        if (receiver.IsKeyDown(KEY_KEY_D) && deadtime <= 0) {
            dx += speed;
            if (kren > -45)kren -= 400 * fdt;
        } else if (receiver.IsKeyDown(KEY_KEY_A) && deadtime <= 0) {
            dx -= speed;
            if (kren < 45)kren += 400 * fdt;
        } else if (kren > 0)kren -= 400 * fdt;
        else if (kren < 0)kren += 400 * fdt;

        if (kren > -1 && kren < 1)kren = 0;
        if (receiver.IsKeyDown(KEY_KEY_W) && deadtime <= 0) dy += speed;
        if (receiver.IsKeyDown(KEY_KEY_S) && deadtime <= 0) dy -= speed;
        if (receiver.IsKeyDown(KEY_ESCAPE))device->closeDevice();
        dx *= friction;
        dy *= friction;
        if (posx < 800 && dx > 0)posx += dx * fdt;
        if (posx > -200 && dx < 0)posx += dx * fdt;
        if (posy < 200 && dy > 0)posy += dy * fdt;
        if (posy > -230 && dy < 0)posy += dy * fdt;

        if (deadtime <= 0) {
            if (posx < 100 || posx > 600) {

                anode->setVisible(false);
                live--;
                deadtime = 4000 * fdt;
                ps2->setPosition(vector3df(anodePosition));
            }


            for (int i = 0; i < en_num_dr; i++) {
                vector3df enpo = enemy[i]->getPosition();
                if (anodePosition.X > enpo.X - 100 && anodePosition.X < enpo.X + 100 &&
                    anodePosition.Y > enpo.Y - 100 && anodePosition.Y < enpo.Y + 100 &&
                    anodePosition.Z > enpo.Z - 100 && anodePosition.Z < enpo.Z + 100) {
                    anode->setVisible(false);
                    live--;
                    deadtime = 4000 * fdt;
                    ps2->setPosition(vector3df(anodePosition));
                }
            }
        }
    }
    mid();
    anode->setRotation(vector3df(0, 0, kren));
    anode->setPosition(vector3df(posx, posy, 400));
}


void mid() {
    int l = 0;
    for (int i = 0; i < 15; i++) {
        for (int f = -2500; f < 2500; f += 300) {
            cube[l]->setPosition(vector3df(f, -320, i * 300 - sme));
            l++; //n
        }
        colonod[i]->setPosition(vector3df(0, 0, i * 300 - sme));
    }
    for (int i = 15; i < 30; i++) {
        colonod[i]->setPosition(vector3df(800, 0, (i - 15) * 300 - sme));
    }
    if (sme > 600)sme = 0;
    sme += speedrun;
}


void crush() {
    vector3df rot = ps2->getRotation();
    ps2->setRotation(rot + vector3df(0, 0, 1600 * fdt));
    deadtime -= 1 * fdt;
    if (deadtime <= 0) {
        posx = 330;
        posy = -150;
        if (live <= 0) {
            start = false;
            camstat = false;
            ini();
        }
    }
    cam->setPosition(
            vector3df(500 - posx / 2 + rand() % 20 - rand() % 20, -posy / 2 + rand() % 20 - rand() % 20, -200));

}

void respawn() {

}

