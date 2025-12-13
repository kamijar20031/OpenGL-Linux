#ifndef OBJECT_H
#define OBJECT_H

#include "model.h"
class ingameObject
{
protected:
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
	void applyForce(glm::vec3 force);
	void resetForce();
	glm::vec3 getVelocity();
	void processPhysics(float dt);
	virtual void process(float dt, Shaders* shader, Camera* camera);
	virtual glm::vec3 getMagnitudeFromCenter(glm::vec3 center) = 0;
	virtual float getSize() = 0;
	virtual void checkCollisionWithDomain(glm::vec3 center, float border)= 0;
	
};
#endif