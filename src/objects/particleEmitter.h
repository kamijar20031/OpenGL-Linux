#ifndef PARTICLE_EMMITER_H_

#define PARTICLE_EMMITER_H_

#include "particle.h"

class ParticleEmitter : public Ball
{
protected:
    modelImporter* importer;
    int speed;
    double timeToLive;
    double time = 0.0;
    double particleLifespan;
    long maxParticles;
    virtual void generateARandomParticle();
public:
    std::vector<std::shared_ptr<Particle>> particles;
    ~ParticleEmitter() = default;
    ParticleEmitter(modelImporter *importer, glm::vec3 position, double timeToLive, double particleLifespan, long numParticles, int generationSpeed=5) : Ball(importer, 1.0f, position, glm::vec3(0.0f), glm::vec3(0.0f)), importer(importer), timeToLive(timeToLive), particleLifespan(particleLifespan), maxParticles(numParticles), speed(generationSpeed) {}
    void process(float dt, Shaders* shader, Camera* camera);
};

#endif