#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "object.h"

class Particle : public ingameObject
{
    double timeToBeAlive;
    double timeAlive = 0.0;
public:
    ~Particle() = default;
    Particle(modelImporter *importer, glm::vec3& position, glm::vec3& color,glm::vec3& startVel, double timeToBeAlive);
    void process(float dt, Shaders* shader, Camera* camera);
    glm::vec3 getMagnitudeFromCenter(glm::vec3 center);
    void checkCollisionWithDomain(glm::vec3 center, float border);
    float getSize();

};

#endif