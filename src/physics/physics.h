#ifndef PHYSICS_H
#define PHYSICS_H

#include "snowEmitter.h"
#include "fireworkEmitter.h"
#include "rectangular.h"
#include "cone.h"
#include "threadPool.h"
#include "softRectangular.h"
#include <unordered_map>
#include <unordered_set>
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
    std::vector <std::shared_ptr<ParticleEmitter>> particleEmitters;
    std::vector <std::shared_ptr<SoftBody>> softBodies;
    std::unordered_map<CellKey, std::vector<GameObject*>, CellKeyHash> grid;
    ThreadPool pool;


    template<typename T>
    void preprocessVector(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shaderProgram, Camera* camera);
    void applyForceGrav(GameObject* object);
    void applyForceAeroDyn(GameObject* object);
    static void applyElasticForceForSoftBody(SoftBody* body);
    template<typename T>
    void applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera);
    void parseCollisionsNonGrid(GameObject* o1);
    void applyCollision(GameObject* o1, GameObject* o2);
    void applyFrictionOnCollision(GameObject* o1, GameObject* o2, glm::vec3 n, float j, float invMassA, float invMassB);
    void addElementToGrid(GameObject* o);
    void refreshGrid(float fpsTime, Shaders* shader, Camera* camera);

    void createRandomBall(modelImporter* importer, glm::vec3 offset, int randCount, float division, glm::vec3 speed = glm::vec3(0.0f));
    void createBoundingBox(modelImporter* importer, glm::vec3 pos);

    void testingSetting(modelImporter* importer, Shaders* shaderProgram);
    void christmasSetting(modelImporter* importer, Shaders* shaderProgram);
    void softBodyTestSetting(modelImporter* importer, Shaders* shaderProgram);

public:
    PhysicsModule() {};
    PhysicsModule(modelImporter* importer, Shaders* shaderprogram);
    void checkCollisions(GameObject* o1, GameObject* o2);
    void process(float fpsTime, Shaders* shaderProgram, Camera* camera);
    void addNewGravityCenter(glm::vec3 pos);
    static bool guiEnabled;
    static bool gravity;
	static bool aero;
    static float stiffness;
    static float mu;
    static float borderOfDomain;
    glm::vec3 centerOfDomain;
    std::vector <glm::vec3> gravityPoints;

};

#endif