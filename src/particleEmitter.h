#ifndef PARTICLE_EMMITER_H_

#define PARTICLE_EMMITER_H_

#include "particle.h"

class ParticleEmitter : public ingameObject
{
    modelImporter* importer;
    double timeToLive;
    double time = 0.0;
    double particleLifespan;
    long maxParticles;
    long currentNumParticles=0;
    std::vector<Particle*> particles;
    void generateARandomParticle();
public:
    ~ParticleEmitter();
    ParticleEmitter(modelImporter *importer, glm::vec3 position, double timeToLive, double particleLifespan, long numParticles);
    void process(float dt, Shaders* shader, Camera* camera);
    glm::vec3 getMagnitudeFromCenter(glm::vec3 center);
    void checkCollisionWithDomain(glm::vec3 center, float border);
    float getSize();
};

#endif