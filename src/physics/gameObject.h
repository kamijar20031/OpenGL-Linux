#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "model.h"
#include "colliders.h"
using CellKey = std::tuple<int,int,int>;

enum CollisionType {
    COLLIDE_NONE   = 0,
    COLLIDE_SOLID  = 1 << 0,
    COLLIDE_PARTICLE = 1 << 1
};

class GameObject
{
protected:
    bool visible;
	static int elementsNum;
	long id;
	bool deleted = false;
    float time;
    glModel model;
public:
    uint32_t collisionLayer;
    uint32_t collisionMask;
    PhysicsBody body;
    std::shared_ptr<Collider> colliders;
    GameObject(const char* name, modelImporter *importer, bool visible=true);
    virtual ~GameObject() = default;
    virtual void process(float dt, Shaders* shader, Camera* camera);
    bool isDeleted() {return deleted;}
	long getID() {return this->id;}
    float getSize();

};

#endif