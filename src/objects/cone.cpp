#include "cone.h"

Cone::Cone(modelImporter *importer, float radius, float height, glm::vec3 position, glm::vec3 velocity,glm::vec3 color, bool isStatic, bool visible) : GameObject("resources/models/cone/cone.obj", importer, visible)
{
    this->body = PhysicsBody(position, velocity,(radius*radius*height)/250.0f, isStatic);
    this->model.scale = glm::vec3(0.01f*radius,0.005f*height,0.01f*radius);
    this->model.defColor = color;
    this->colliders =std::make_shared<ConvexCollider>(glm::vec3(0.01f*radius, 0.01f*height, 0.0f), ConvexType::Cone);
}
