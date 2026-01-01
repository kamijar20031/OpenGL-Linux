#ifndef PHYSICS_BODY_H_
#define PHYSICS_BODY_H_

#include <glm/glm.hpp>
#include <vector>
#include <limits.h>

class PhysicsBody
{
	float mass;
    glm::vec3 pos;
    glm::vec3 acc;
	glm::vec3 vel;
    bool isStatic;

public:
    PhysicsBody() {};
    PhysicsBody(glm::vec3 position, glm::vec3 velocity, float mass, bool _static) : pos(position), vel(velocity), mass(mass), isStatic(_static), acc(glm::vec3(0.0f)) {}
    void calcPhysics(float dt);
    void applyForce(glm::vec3 force);
    void resetForce();

    glm::vec3 getPosition() {return this->pos;}
    glm::vec3 getVelocity() {return this->vel;}
    float getMass() {return this->mass;}

    
    void setPosition(glm::vec3 position) {this->pos=position;}
    void setVelocity(glm::vec3 velocity) {this->vel=velocity;}
};

#endif

