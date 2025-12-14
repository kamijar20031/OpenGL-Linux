#include "object.h"

int ingameObject::elementsNum = 0;

ingameObject::ingameObject(const char* name, modelImporter* importer) 
{
	
	this->id = ingameObject::elementsNum;
	ingameObject::elementsNum+=1;
	this->time = 0.0;
	this->mass = 0.1f;
	this->vel = glm::vec3(0.0f,0.0f,0.0f);
	this->model = glModel(name, importer);
	this->model.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acc = glm::vec3(0.0f,0.0f,0.0f);
}

void ingameObject::applyCollision(ingameObject* o)
{
	glm::vec3 delta = this->getCenter() - o->getCenter();
	float len2 = glm::dot(delta, delta);
	if (len2 < 1e-6f)
		return;
	glm::vec3 n = delta / (float)sqrt(len2);
	float v1n = glm::dot(this->getVelocity(), n);
	float v2n = glm::dot(o->getVelocity(), n);

	float relVel = glm::dot(this->getVelocity() - o->getVelocity(), n);
	if (relVel>0)
		return;
	glm::vec3 v1n_vec = v1n * n;
	glm::vec3 v2n_vec = v2n * n;

	glm::vec3 v1t = this->getVelocity() - v1n_vec;
	glm::vec3 v2t = o->getVelocity() - v2n_vec;

	float v1ni = ( (this->getMass() - o->getMass()) * v1n + 2*o->getMass() * v2n ) / (this->getMass() + o->getMass());
	float v2ni = ( (o->getMass() - this->getMass()) * v2n + 2*this->getMass() * v1n ) / (this->getMass() + o->getMass());

	this->setVelocity(v1t + v1ni * n);
	o->setVelocity(v2t + v2ni * n);
	
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