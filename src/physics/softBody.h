#ifndef SOFT_BODY_H_
#define SOFT_BODY_H_

#include "softBodyVert.h"
struct SoftBodySpring
{
    SoftBodyVertex* v1;
    SoftBodyVertex* v2;

    float mRestLength;
    float mStiffness;
    float mDamping;

    SoftBodySpring(SoftBodyVertex* _v1, SoftBodyVertex* _v2, float damping, float stiffness, float restLength) :  mDamping(damping), mStiffness(stiffness), mRestLength(restLength) {v1 = _v1; v2=_v2;}
};

class SoftBody : public GameObject
{
protected:
    std::vector<std::shared_ptr<SoftBodySpring>> springs;

public:
    std::vector<std::shared_ptr<SoftBodyVertex>> vertices;
    SoftBody(modelImporter* importer) : GameObject("resources/models/ball/ball.obj", importer, false) {};
    virtual void process(float dt, Shaders* shader, Camera* camera);
};

#endif