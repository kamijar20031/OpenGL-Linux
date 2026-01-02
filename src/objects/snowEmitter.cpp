#include "snowEmitter.h"

void SnowEmitter::generateARandomParticle()
{
    int randCount = 50;
    float speedDiv = 25.0f;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 speed = glm::vec3(0.6f*(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv, -(rand()%randCount+1)/speedDiv,0.6f*(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv);

    this->particles.emplace_back(std::make_shared<Particle>(importer, this->body.getPosition(), color, speed, this->particleLifespan));

}