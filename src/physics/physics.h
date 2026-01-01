#ifndef PHYSICS_H
#define PHYSICS_H

#include "particleEmitter.h"
#include "rectangular.h"
#include <unordered_map>




using CellKey = std::tuple<int,int,int>;
extern float cellSize;

struct CellKeyHash 
{
    std::size_t operator()(const CellKey& k) const
    {
        int x = std::get<0>(k);
        int y = std::get<1>(k);
        int z = std::get<2>(k);
        return (x * 73856093) ^ (y * 19349663) ^ (z * 83492791);
    }
};

class PhysicsModule
{
    std::vector <std::shared_ptr<GameObject>> objects;
    std::vector <std::shared_ptr<ParticleEmitter>> 
    particleEmitters;
    std::unordered_map<CellKey, std::vector<GameObject*>, CellKeyHash> grid;
    template<typename T>
    void preprocessVector(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera);
    void applyForceGrav(GameObject* object);
    void applyForceAeroDyn(GameObject* object);
    template<typename T>
    void applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera);
    void simpleCollision(GameObject* o1, GameObject* o2);
    void applyCollision(GameObject* o1, GameObject* o2);
    void applyCollisions(GameObject* o1, GameObject* o2);
    void addElementToGrid(GameObject* o);
    void refreshGrid(float fpsTime, Shaders* shader, Camera* camera);
public:
    PhysicsModule() {};
    PhysicsModule(modelImporter* importer, Shaders* shaderprogram);
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