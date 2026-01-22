#include "softBodyVert.h"
#include <chrono>
#include <thread>
void SoftBodyVertex::process(float dt, Shaders* shader, Camera* camera)
{
    this->time += dt;
    if (firstFrame)
    {
        glm::vec3 pos = body.getPosition();
        glm::vec3 v0  = body.getVelocity();
        prevPos = pos - v0 * dt;
        this->body.resetForce();
        firstFrame = false;
    }
    else
    {
        this->body.setVelocity((this->body.getPosition() - prevPos));
        glm::vec3 tempPos = this->body.getPosition();
        this->body.setPosition(2.0f*this->body.getPosition() - prevPos + this->body.getAcceleration()*dt*dt);
        prevPos = tempPos;
        this->body.resetForce();
    }

    if (visible)
    {   
        this->model.translation = this->body.getPosition();
        this->model.draw(shader, camera);
    }
}