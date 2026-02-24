#ifndef SOFT_BODY_VERT_H_
#define SOFT_BODY_VERT_H_

#include "ball.h"

class SoftBodyVertex : public Ball
{
    glm::vec3 prevPos =glm::vec3(0.0f);
    bool firstFrame = true;
public:
    SoftBodyVertex(modelImporter *importer, glm::vec3 position, glm::vec3 velocity,glm::vec3 color, bool isStatic= false) : Ball(importer, 2.0f, position, velocity, color, isStatic) 
    {
        this->prevPos = position;
        collisionLayer = COLLIDE_SOLID;
        collisionMask = COLLIDE_SOLID;
    }
    void process(float dt, Shaders* shader, Camera* camera);
    glm::vec3 getPreviousPosition() {return prevPos;}

};

#endif