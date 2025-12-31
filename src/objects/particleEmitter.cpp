#include "particleEmitter.h"

void ParticleEmitter::generateARandomParticle()
{
    int randCount = 50;
    float speedDiv = 25.0f;

    glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);
    glm::vec3 speed = glm::vec3((1-2*(rand()%2))*(rand()%randCount+1)/speedDiv, (1-2*(rand()%2))*(rand()%randCount+1)/speedDiv,(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv);

    this->particles.emplace_back(std::make_shared<Particle>(importer, this->body.getPosition(), color, speed, this->particleLifespan));

}

void ParticleEmitter::process(float dt, Shaders* shader, Camera* camera)
{
    this->time+=dt;
    if (this->timeToLive!=0.0 && this->time>=this->timeToLive)
    {
        this->deleted = true;
    }
    else
    {
        if (particles.size() <maxParticles)
        {
            std::cout << particles.size() << std::endl;
            for (int i=0; i<5; i++)
                this->generateARandomParticle();
        }
    }

}
