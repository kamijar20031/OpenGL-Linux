#ifndef SNOW_EMMITER_H_

#define SNOW_EMMITER_H_

#include "particleEmitter.h"

class SnowEmitter : public ParticleEmitter
{
void generateARandomParticle();
    public:
SnowEmitter(modelImporter *importer, glm::vec3 position, double timeToLive, long numParticles, float particleLifespan, int speed = 1) : ParticleEmitter(importer, position, timeToLive, particleLifespan, numParticles, speed) {}


};

#endif