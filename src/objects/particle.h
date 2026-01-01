#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "ball.h"

class Particle : public Ball
{
    double timeToBeAlive;
    double timeAlive = 0.0;
public:
    ~Particle() = default;
    Particle(modelImporter *importer, glm::vec3 position, glm::vec3 color, glm::vec3 startVel, double _timeToBeAlive) : Ball(importer, 2.0f, position, startVel, color), timeToBeAlive(_timeToBeAlive) {}
    void process(float dt, Shaders* shader, Camera* camera);
    virtual bool collides() {return false;}

};

#endif