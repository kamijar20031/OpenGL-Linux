#ifndef OBJECT_H
#define OBJECT_H

#include "model.h"
class ingameObject
{
protected:
	static int elementsNum;
	long id;
	bool deleted = false;
	float time;
	float mass;
	glm::vec3 acc;
	glm::vec3 vel;
	glModel model;

public:
	virtual ~ingameObject() = default;
	ingameObject(const char* name, modelImporter *importer);
	bool isDeleted() {return deleted;}
	void applyCollision(ingameObject* o);
	void applyForce(glm::vec3 force);
	void resetForce();
	void setVelocity(glm::vec3 vel) {this->vel = vel;}
	float getMass() {return this->mass;}
	long getID() {return this->id;}
	glm::vec3 getVelocity();
	glm::vec3 getCenter() {return this->model.translation;}
	void processPhysics(float dt);
	virtual void process(float dt, Shaders* shader, Camera* camera);
	virtual glm::vec3 getMagnitudeFromCenter(glm::vec3 center) = 0;
	virtual float getSize() = 0;
	virtual void checkCollisionWithDomain(glm::vec3 center, float border)= 0;
	
};
#endif