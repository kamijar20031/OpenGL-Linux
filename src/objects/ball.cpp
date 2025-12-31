#include "ball.h"


Ball::Ball(modelImporter *importer, float size, glm::vec3 position, glm::vec3 velocity, glm::vec3 color, bool isStatic) : GameObject("resources/models/ball/ball.obj", importer)
{
    this->body = PhysicsBody(position, velocity,(size*size*size)/1000.0f, isStatic);
    this->model.scale = glm::vec3(0.01f,0.01f,0.01f)*size;
    this->model.defColor = color;
    this->colliders =std::make_shared<SphereCollider>(size*0.01f, position);
}

void Ball::process(float dt, Shaders* shader, Camera* camera)
{
    GameObject::process(dt, shader, camera);
}