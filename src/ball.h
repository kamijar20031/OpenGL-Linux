#ifndef BALL_H
#define BALL_H


#include "object.h"
#include <numbers>

class Ball : public ingameObject
{
    float size;
    public:
    ~Ball() = default;
    Ball(modelImporter *importer, float size, glm::vec3 position, glm::vec3 velocity,glm::vec3 color);
    void process(float dt, Shaders* shader, Camera* camera);
    glm::vec3 getMagnitudeFromCenter(glm::vec3 center);
    void checkCollisionWithDomain(glm::vec3 center, float border);
    float getSize();
};

#endif