#ifndef COLLIDER_H_
#define COLLIDER_H_

#include "physicsBody.h"

class Collider;
class SphereCollider;
class ConvexCollider;

class Collider
{
    glm::vec3 localOffset;
public:
    Collider(glm::vec3 offset) : localOffset(offset) {}
    virtual ~Collider();
    virtual bool testCollision( PhysicsBody* a,  Collider* b,  PhysicsBody* bBody)  = 0;
    virtual bool collidedWithSphere( PhysicsBody* a,  SphereCollider* b,  PhysicsBody* bBody)  = 0;
    virtual bool collidedWithConvex( PhysicsBody* a,  ConvexCollider* b,  PhysicsBody* bBody)  = 0;
    virtual glm::vec3 support(glm::vec3 d)  = 0;
    glm::vec3 getLocalOffset() {return localOffset;}
    virtual glm::vec3 getNormal() {return glm::vec3(2.0f,2.0f,2.0f);}
    virtual float getSize() = 0;
};

class SphereCollider : public Collider 
{
    glm::vec3 center;
    float radius;
    
public:
    SphereCollider(float r, glm::vec3 position, glm::vec3 offset=glm::vec3(0.0f)) : Collider(offset), center{position}, radius(r) {}
    bool testCollision( PhysicsBody* a,  Collider* b,  PhysicsBody* bBody) ;
    bool collidedWithSphere( PhysicsBody* a,  SphereCollider* b,  PhysicsBody* bBody) ;
    bool collidedWithConvex( PhysicsBody* a,  ConvexCollider* b,  PhysicsBody* bBody) ;
    glm::vec3 support(glm::vec3 d) ;
    float getRadius() {return this->radius;}
    float getSize() {return getRadius();}
    
};


enum class ConvexType {
    Box,
    Cone,
    Capsule
};

class ConvexCollider : public Collider
{
    ConvexType type;
    // a b c - Box
    // R h - Cone
    // r h - Capsule
    glm::vec3 values;
    glm::vec3 lastDirectionUsed;
public:
    ConvexCollider(glm::vec3 extents,ConvexType typeOfConvex,  glm::vec3 offset=glm::vec3(0.0f)) : Collider(offset), values(extents), type(typeOfConvex) {}
    bool testCollision( PhysicsBody* a,  Collider* b,  PhysicsBody* bBody) ;
    bool collidedWithSphere( PhysicsBody* a,  SphereCollider* b,  PhysicsBody* bBody) ;
    bool collidedWithConvex( PhysicsBody* a,  ConvexCollider* b,  PhysicsBody* bBody) ;
    glm::vec3 support(glm::vec3 d) ;
    void setLastDirection(glm::vec3 dir) {this->lastDirectionUsed = dir;}
    glm::vec3 getNormal() {return glm::normalize(lastDirectionUsed);}
    float getSize() 
    {
        return std::max(values.x, std::max(values.y, values.z));
    }
};


#endif