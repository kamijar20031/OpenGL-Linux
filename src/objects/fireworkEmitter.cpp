#include "fireworkEmitter.h"

void FireworkEmitter::generateARandomParticle()
{
    int randCount = 50;
    float speedDiv = 25.0f;

    glm::vec3 speed = glm::vec3((1-2*(rand()%2))*(rand()%randCount+1)/speedDiv, (1-2*(rand()%2))*(rand()%randCount+1)/speedDiv,(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv);

    this->particles.emplace_back(std::make_shared<Particle>(importer, this->body.getPosition(), color, speed, this->particleLifespan, false));

}