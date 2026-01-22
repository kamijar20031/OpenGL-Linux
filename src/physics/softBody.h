#ifndef SOFT_BODY_H_
#define SOFT_BODY_H_

#include "softBodyVert.h"
struct SoftBodySpring
{
    std::shared_ptr<SoftBodyVertex> v1;
    std::shared_ptr<SoftBodyVertex> v2;

    float mRestLength;
    float mRatio;

    SoftBodySpring( std::shared_ptr<SoftBodyVertex> _v1,  std::shared_ptr<SoftBodyVertex> _v2, float restLength, float ratio=1.0f) : mRestLength(restLength), mRatio(ratio) {v1 = _v1; v2=_v2; }
};

class SoftBody : public GameObject
{
protected:
    std::vector<std::shared_ptr<SoftBodySpring>> springs; 
public:
    std::vector<std::shared_ptr<SoftBodyVertex>> vertices;
    SoftBody(modelImporter* importer) : GameObject("resources/models/ball/ball.obj", importer, false) {};
    virtual void process(float dt, Shaders* shader, Camera* camera);
    void processElasticForces(float stiffness);
};

#endif