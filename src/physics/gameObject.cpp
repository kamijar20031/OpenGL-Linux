#include "gameObject.h"
int GameObject::elementsNum = 0;
void GameObject::process(float dt, Shaders* shader, Camera* camera)
{
    this->time += dt;
    this->body.calcPhysics(dt);
    if (visible)
    {
        this->model.translation = this->body.getPosition();
        this->model.draw(shader, camera);
    }

}

GameObject::GameObject(const char* name, modelImporter* importer, bool visible)
{
    collisionLayer = COLLIDE_SOLID;
    collisionMask = COLLIDE_SOLID | COLLIDE_PARTICLE;
    this->visible= visible;
    this->model = glModel(name, importer);
    this->id = GameObject::elementsNum;
	GameObject::elementsNum+=1;
}

float GameObject::getSize()
{
    return colliders->getSize();
}
