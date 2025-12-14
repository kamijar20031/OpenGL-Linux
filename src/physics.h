#ifndef PHYSICS_H
#define PHYSICS_H

#include "particleEmitter.h"
#include"ball.h"
class PhysicsModule
{
    std::vector <std::shared_ptr<ingameObject>> objects;
    std::vector <std::shared_ptr<ParticleEmitter>> particleEmitters;
    void applyForceGrav(ingameObject* object);
    void applyForceAeroDyn(ingameObject* object);
public:
    PhysicsModule() {};
    PhysicsModule(modelImporter* importer, Shaders* shaderprogram);
    template<typename T>
    void applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements,  float fpsTime, Shaders* shader, Camera* camera);
    void process(float fpsTime, Shaders* shaderProgram, Camera* camera);
    void addNewGravityCenter(glm::vec3 pos);
    bool gravity = false;
	bool aero = false;
    float mu = -4.0f;
    float borderOfDomain;
    glm::vec3 centerOfDomain;
    std::vector <glm::vec3> gravityPoints;

};

#endif