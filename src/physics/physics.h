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
    static std::vector <std::shared_ptr<GameObject>> objects;
    static std::vector <std::shared_ptr<ParticleEmitter>> particleEmitters;
    static std::vector <std::shared_ptr<SoftBody>> softBodies;
    static std::unordered_map<CellKey, std::vector<GameObject*>, CellKeyHash> grid;

    template<typename T>
    static void preprocessVector(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shaderProgram, Camera* camera);
    static void applyForceGrav(GameObject* object);
    static void applyForceAeroDyn(GameObject* object);
    static void applyElasticForceForSoftBody(SoftBody* body);
    template<typename T>
    static void applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera);
    static void parseCollisionsNonGrid(GameObject* o1);
    static void applyCollision(GameObject* o1, GameObject* o2);
    static void applyFrictionOnCollision(GameObject* o1, GameObject* o2, glm::vec3 n, float j, float invMassA, float invMassB);
    static void addElementToGrid(GameObject* o);

    static void createRandomBall(modelImporter* importer, glm::vec3 offset, int randCount, float division, glm::vec3 speed = glm::vec3(0.0f));
    static void createBoundingBox(modelImporter* importer, glm::vec3 pos);

    static void testingSetting(modelImporter* importer, Shaders* shaderProgram);
    static void christmasSetting(modelImporter* importer, Shaders* shaderProgram);
    static void softBodyTestSetting(modelImporter* importer, Shaders* shaderProgram);

public:
    static void init(modelImporter* importer, Shaders* shaderprogram);
    static void checkCollisions(GameObject* o1, GameObject* o2);
    static void process(float fpsTime, Shaders* shaderProgram, Camera* camera);
    static void addNewGravityCenter(glm::vec3 pos);
    static bool guiEnabled;
    static bool gravity;
	static bool aero;
    static float stiffness;
    static float mu;
    static float borderOfDomain;
    static glm::vec3 centerOfDomain;
    static std::vector <glm::vec3> gravityPoints;

};

#endif