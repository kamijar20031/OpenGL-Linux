#include "physics.h"

void PhysicsModule::applyForceGrav(ingameObject* object)
{
    
    float epsilon = 0.2f;
	for (auto& i : gravityPoints)
	{
		glm::vec3 mag3 = object->getMagnitudeFromCenter(i);
		float magL = glm::length(mag3);
		object->applyForce(-mag3/(float)(pow(magL + epsilon, 3.0f)));
	}
		
}

void PhysicsModule::applyForceAeroDyn(ingameObject* object)
{
    object->applyForce(-6.0f*object->getVelocity()*(float)(pow(10,mu))*object->getSize()*(float)(std::numbers::pi));
}



PhysicsModule::PhysicsModule(modelImporter* importer, Shaders* shaderProgram)
{
    int randCount = 50;
	float offset = -10.0f;
	borderOfDomain = 4.0f;
	centerOfDomain = glm::vec3(0.0f,0.0f,offset);
	float division = (float)(randCount/2)/(borderOfDomain-0.1f);
	float speedDiv = 45.0f;
	glUniform3f(glGetUniformLocation(shaderProgram->getID(), "lightPos"), 0.0f, 0.0f, offset);
	gravityPoints.push_back(glm::vec3(centerOfDomain));
	objects.push_back(new ParticleEmitter(importer, glm::vec3(0.0f,0.0f,-10.0f), 0.0, 5.0, 1000));
	// for (int i=0; i<1000;i++)
	// {
	// 	float size = (rand()%randCount)/10.0f + 0.3;
	// 	glm::vec3 position = glm::vec3((rand()%randCount-randCount/2)/division,(rand()%randCount-randCount/2)/division,offset+ (rand()%randCount-randCount/2)/division);
	// 	glm::vec3 speed = glm::vec3((1-2*rand()%2)*(rand()%randCount+1)/speedDiv, (1-2*rand()%2)*(rand()%randCount+1)/speedDiv,(1-2*rand()%2)*(rand()%randCount+1)/speedDiv);
	// 	glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

	// 	objects.push_back(new Ball(importer, size, position, speed,color));
	// }
}
void PhysicsModule::process(float fpsTime, Shaders* shaderProgram, Camera* camera)
{
    for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->resetForce();
		if (gravity)
			this->applyForceGrav(objects[i]);
		if (aero)
			this->applyForceAeroDyn(objects[i]);
		objects[i]->checkCollisionWithDomain(centerOfDomain,borderOfDomain);
		objects[i]->process(fpsTime, shaderProgram, camera);
	}
}
void PhysicsModule::addNewGravityCenter(glm::vec3 pos)
{
	gravityPoints.push_back(pos);
}