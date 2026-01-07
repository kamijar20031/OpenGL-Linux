#ifndef PHYSICS_H
#define PHYSICS_H

#include "snowEmitter.h"
#include "rectangular.h"
#include "cone.h"
#include <unordered_map>
#include <numbers>

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
    void parseCollisionsNonGrid(GameObject* o1);
    void applyCollision(GameObject* o1, GameObject* o2);
    void checkCollisions(GameObject* o1, GameObject* o2);
    void addElementToGrid(GameObject* o);
    void refreshGrid(float fpsTime, Shaders* shader, Camera* camera);

    void createRandomBall(modelImporter* importer, glm::vec3 offset, int randCount, float division, glm::vec3 speed = glm::vec3(0.0f));
    void createBoundingBox(modelImporter* importer, float borderOfDomain, glm::vec3 pos);

    void testingSetting(modelImporter* importer, Shaders* shaderProgram);
    void christmasSetting(modelImporter* importer, Shaders* shaderProgram);

public:
    PhysicsModule() {};
    PhysicsModule(modelImporter* importer, Shaders* shaderprogram);
    void process(float fpsTime, Shaders* shaderProgram, Camera* camera);
    void addNewGravityCenter(glm::vec3 pos);
    bool guiEnabled;
    bool gravity = false;
	bool aero = false;
    float mu = -4.0f;
    float borderOfDomain;
    glm::vec3 centerOfDomain;
    std::vector <glm::vec3> gravityPoints;

};

#endif