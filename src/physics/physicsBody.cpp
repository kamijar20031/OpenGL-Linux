#include "physicsBody.h"

void PhysicsBody::calcPhysics(float dt)
{
    if (!this->isStatic)
    {
        this->vel+= dt*acc;
        this->pos+= dt*vel;
        this->acc = glm::vec3(0.0f);
    }
}
void PhysicsBody::applyForce(glm::vec3 force)
{
    this->acc = force/mass;
}
void PhysicsBody::resetForce()
{
    this->acc = glm::vec3(0.0f);
}

