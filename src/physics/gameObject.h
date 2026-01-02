#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "model.h"
#include "colliders.h"
using CellKey = std::tuple<int,int,int>;

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
    PhysicsBody body;
    std::shared_ptr<Collider> colliders;
    CellKey primaryCell;
    GameObject(const char* name, modelImporter *importer, bool visible=true);
    bool collidesWith(GameObject* o) {return o->collides();}
    virtual ~GameObject() = default;
    virtual bool collides() {return true;}
    virtual void process(float dt, Shaders* shader, Camera* camera);
    bool isDeleted() {return deleted;}
	long getID() {return this->id;}
    float getSize();

};

#endif