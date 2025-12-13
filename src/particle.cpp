#include "particle.h"

Particle::Particle(modelImporter *importer, glm::vec3& position, glm::vec3& color, glm::vec3& startVel, double timeToBeAlive) : ingameObject("resources/models/ball/ball.obj", importer)
{
    this->model.scale = glm::vec3(0.01f,0.01f,0.01f);
    this->model.translation = position;
    this->vel = startVel;
    this->model.defColor = color;
    this->timeToBeAlive = timeToBeAlive;
}

void Particle::process(float dt, Shaders* shader, Camera* camera)
{
    this->timeAlive+=dt;
    if (this->timeToBeAlive!=0.0&&this->timeAlive>=this->timeToBeAlive)
    {
        this->deleted = true;
    }
    else
    {
        this->processPhysics(dt);
        ingameObject::process(dt, shader, camera);
    }

}

glm::vec3 Particle::getMagnitudeFromCenter(glm::vec3 center)
{
    return (this->model.translation - center);
}
void Particle::checkCollisionWithDomain(glm::vec3 center, float border)
{
    for (int i=0; i<3; i++)
    {
        if (this->model.translation[i]  > center[i]+border)
        {
            this->model.translation[i] = (center[i]+border);
            vel[i] = -vel[i];
        }
        if (this->model.translation[i] < center[i]-border)
        {
            this->model.translation[i] = (center[i]-border);
            vel[i] = -vel[i];
        }
    }
    
}
float Particle::getSize()
{
    return 0.0f;
}