#include "ball.h"


Ball::Ball(modelImporter *importer, float size, glm::vec3 position, glm::vec3 velocity, glm::vec3 color) : ingameObject("resources/models/ball/ball.obj", importer)
{
    this->size = size;
    this->mass = (size*size*size)/1000.0f;
    this->model.scale = glm::vec3(0.01f,0.01f,0.01f)*size;
    this->model.translation = position;
    this->vel=velocity;
    this->model.defColor = color;
}

float Ball::getSize()
{
    return this->size*0.01f;
}

glm::vec3 Ball::getMagnitudeFromCenter(glm::vec3 center)
{
    return (this->model.translation - center);
}

void Ball::checkCollisionWithDomain(glm::vec3 center, float border)
{
    for (int i=0; i<3; i++)
    {
        // Promien tej kuli wynosi 0.01 i jest skalowany przez size
        if (this->model.translation[i] + size*0.01f > center[i]+border)
        {
            this->model.translation[i] = (center[i]+border) -size*0.01f;
            vel[i] = -vel[i];
        }
        if (this->model.translation[i] - size*0.01f < center[i]-border)
        {
            this->model.translation[i] = (center[i]-border) + size*0.01f;
            vel[i] = -vel[i];
        }
    }
}

void Ball::process(float dt, Shaders* shader, Camera* camera)
{
    this->processPhysics(dt);
    ingameObject::process(dt, shader, camera);

}