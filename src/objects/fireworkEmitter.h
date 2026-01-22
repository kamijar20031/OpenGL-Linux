#ifndef FIREWORK_EMMITER_H_

#define FIREWORK_EMMITER_H_

#include "particleEmitter.h"

class FireworkEmitter : public ParticleEmitter
{
    glm::vec3 color;
void generateARandomParticle();
    public:
FireworkEmitter(modelImporter *importer, glm::vec3 position, glm::vec3 color) : ParticleEmitter(importer, position, 0.0f, 3.0f, 200, 20), color{color} {}


};

#endif