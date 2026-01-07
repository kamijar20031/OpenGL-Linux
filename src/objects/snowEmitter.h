#ifndef SNOW_EMMITER_H_

#define SNOW_EMMITER_H_

#include "particleEmitter.h"

class SnowEmitter : public ParticleEmitter
{
void generateARandomParticle();
    public:
SnowEmitter(modelImporter *importer, glm::vec3 position, double timeToLive) : ParticleEmitter(importer, position, timeToLive, 12.0f, 5000, 1) {}


};

#endif