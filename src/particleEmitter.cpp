#include "particleEmitter.h"

ParticleEmitter::ParticleEmitter(modelImporter *importer, glm::vec3 position, double timeToLive, double particleLifespan, long numParticles) : ingameObject("resources/models/ball/ball.obj", importer)
{
    this->importer = importer;
    this->model.translation = position;
    this->timeToLive=timeToLive;
    this->particleLifespan=particleLifespan;
    this->maxParticles=numParticles;
}
ParticleEmitter::~ParticleEmitter()
{
    for (Particle* p : particles)
        delete p;
    particles.clear();
}
void ParticleEmitter::generateARandomParticle()
{
    int randCount = 50;
    float speedDiv = 25.0f;

    glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);
    glm::vec3 speed = glm::vec3((1-2*(rand()%2))*(rand()%randCount+1)/speedDiv, (1-2*(rand()%2))*(rand()%randCount+1)/speedDiv,(1-2*(rand()%2))*(rand()%randCount+1)/speedDiv);

    this->particles.push_back(new Particle(importer, this->model.translation, color, speed, this->particleLifespan));

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
        this->generateARandomParticle();
        this->processPhysics(dt);
        for (auto it = particles.begin(); it != particles.end(); )
        {
            if (!(*it)->isDeleted())
            {
                (*it)->process(dt, shader, camera);
                ++it;
            }
            else
            {
                it = particles.erase(it);
            }
        }
    }

}

glm::vec3 ParticleEmitter::getMagnitudeFromCenter(glm::vec3 center)
{
    return (this->model.translation - center);
}
void ParticleEmitter::checkCollisionWithDomain(glm::vec3 center, float border)
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
float ParticleEmitter::getSize()
{
    return 0.0f;
}