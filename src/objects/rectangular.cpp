#include "rectangular.h"

Rectangular::Rectangular(modelImporter *importer, glm::vec3 size, glm::vec3 position, glm::vec3 velocity, glm::vec3 color, bool isStatic, bool visible) : GameObject("resources/models/cube/cube.obj", importer, visible)
{
    this->body = PhysicsBody(position, velocity,(size.x*size.y*size.z)/250.0f, isStatic);
    this->model.scale = glm::vec3(0.01f*size.x,0.01f*size.y,0.01f*size.z);
    this->model.defColor = color;
    this->colliders =std::make_shared<ConvexCollider>(size*0.01f, ConvexType::Box);
}
