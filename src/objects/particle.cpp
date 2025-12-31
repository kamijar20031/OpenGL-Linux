#include "particle.h"

void Particle::process(float dt, Shaders* shader, Camera* camera)
{
    this->timeAlive+=dt;
    if (this->timeToBeAlive!=0.0&&this->timeAlive>=this->timeToBeAlive)
    {
        this->deleted = true;
    }
    else
    {
        GameObject::process(dt, shader, camera);
    }

}