#include "softBody.h"

void SoftBody::process(float dt, Shaders* shader, Camera* camera)
{
    for (auto& spring: springs)
    {
        glm::vec3 pos2 = spring->v2->body.getPosition();
        glm::vec3 pos1 = spring->v1->body.getPosition();
        glm::vec3 delta = pos2 - pos1;
        float len = glm::length(delta);
        if (len < 1e-6f) continue;
        float diff = (len - spring->mRestLength) / len;
        glm::vec3 f = spring->mStiffness * (len-spring->mRestLength) * glm::normalize(delta);

        float w1 = 1.0f / spring->v1->body.getMass();
        float w2 = 1.0f / spring->v2->body.getMass();
        float wsum = w1 + w2;

        glm::vec3 correction = spring->mStiffness * diff * delta;

        spring->v1->body.setPosition(pos1 + correction * (w1 / wsum));
        spring->v2->body.setPosition(pos2 - correction * (w2 / wsum));

    }
}