#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <irrlicht/irrlicht.h>

class GameObject {
public:
  virtual ~GameObject() = default;

  virtual void update(float dt) = 0;
  virtual void render() = 0;
  virtual irr::scene::ISceneNode *getSceneNode() = 0;
};

#endif /* GAME_OBJECT_H_ */