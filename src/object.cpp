#include "object.h"

ingameObject::ingameObject(const char* name, modelImporter* importer) 
{
	this->time = 0.0;
	this->mass = 1.0f;
	this->vel = glm::vec3(0.0f,0.0f,0.0f);
	this->model = glModel(name, importer);
	this->model.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acc = glm::vec3(0.0f,0.0f,0.0f);
}

glm::vec3 ingameObject::getVelocity()
{
	return this->vel;
}

void ingameObject::applyForce(glm::vec3 force)
{
	this->acc += force/mass;
}

void ingameObject::resetForce()
{
	this->acc = glm::vec3(0.0f);
}

void ingameObject::processPhysics(float dt)
{
	this->time += dt;
	this->vel+= dt*acc;
	this->model.translation+= dt*vel;
}

void ingameObject::process(float dt, Shaders* shader, Camera* camera)
{
	this->model.draw(shader, camera);
}