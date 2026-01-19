#include "fireworkEmitter.h"

void FireworkEmitter::generateARandomParticle()
{
    int randCount = 50;
    float speedDiv = 25.0f;

    glm::vec3 speed = glm::vec3((1-2*(rand()%2))*(rand()%randCount+1)/speedDiv, (1-2*(rand()%2))*(rand()%randCount+1)/speedDiv,(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv);
    glm::vec3 posDifference = glm::vec3((1-2*(rand()%2))*((rand()%(randCount+1))/(float)(randCount))*genRadius, (1-2*(rand()%2))*((rand()%(randCount+1))/(float)(randCount))*genRadius, (1-2*(rand()%2))*((rand()%(randCount+1))/(float)(randCount))*genRadius);
    this->particles.emplace_back(std::make_shared<Particle>(importer, this->body.getPosition() + posDifference, color, speed, this->particleLifespan, false));

}