#ifndef EVENTRECEIVER_H_
#define EVENTRECEIVER_H_

#include <irrlicht/irrlicht.h>

class EventReceiver : public irr::IEventReceiver
{
public:
    EventReceiver() {
        for (irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++i) {
            m_keyIsDown[i] = false;
        }
    }

    virtual bool OnEvent(const irr::SEvent &event) {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            m_keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }

        return false;
    }

    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
        return m_keyIsDown[keyCode];
    }

private:
    bool m_keyIsDown[irr::KEY_KEY_CODES_COUNT];
};

#endif /* EVENTRECEIVER_H_ */