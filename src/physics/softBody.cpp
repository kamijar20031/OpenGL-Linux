#include "softBody.h"



void SoftBody::processElasticForces(float stiffness)
{
    for (auto& spring: springs)
    {
        glm::vec3 pos2 = spring->v2->body.getPosition();
        glm::vec3 pos1 = spring->v1->body.getPosition();
        glm::vec3 delta = pos2 - pos1;
        float len = glm::length(delta);
        if (len < 1e-6f) continue;
        float dif1 = (len - spring->mRestLength);
        float diff  = dif1/len;
        float w1 = 1.0f / spring->v1->body.getMass();
        float w2 = 1.0f / spring->v2->body.getMass();
        float wsum = w1 + w2;
        float stif = stiffness;
        if (dif1>1.3f*spring->mRestLength)
            stif = 0.0f;

        glm::vec3 correction = stif*spring->mRatio * diff * delta;

        if (!spring->v1->body.getIsStatic())
            spring->v1->body.setPosition(pos1 + correction * (w1 / wsum));
        if (!spring->v2->body.getIsStatic())
            spring->v2->body.setPosition(pos2 - correction * (w2 / wsum));

    }
}
void SoftBody::process(float dt, Shaders* shader, Camera* camera)
{
    // Np algorytm face restoration czy cos podobnego
}