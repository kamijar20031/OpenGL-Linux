#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "model.h"
#include "colliders.h"

class GameObject
{
protected:
	static int elementsNum;
	long id;
	bool deleted = false;
    float time;
    glModel model;
public:
    PhysicsBody body;
    std::shared_ptr<Collider> colliders;

    GameObject(const char* name, modelImporter *importer);
    virtual ~GameObject() = default;
    virtual void process(float dt, Shaders* shader, Camera* camera);
    bool isDeleted() {return deleted;}
	long getID() {return this->id;}
    float getSize();

};

#endif