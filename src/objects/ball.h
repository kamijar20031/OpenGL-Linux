#ifndef BALL_H
#define BALL_H


#include "gameObject.h"

class Ball : public GameObject
{
    public:
    ~Ball() = default;
    Ball(modelImporter *importer, float size, glm::vec3 position, glm::vec3 velocity,glm::vec3 color, bool isStatic=false, bool visible=true);
    virtual void process(float dt, Shaders* shader, Camera* camera);
    virtual bool collides() {return true;}
};

#endif