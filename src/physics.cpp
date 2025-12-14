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

bool checkCollisionWithObject(ingameObject* o1, ingameObject* o2)
{

	float r = o1->getSize() + o2->getSize();
	glm::vec3 d = o1->getCenter() - o2->getCenter();
	float dist2 = glm::dot(d, d);
	return dist2 <= r * r;	
}

void PhysicsModule::applyCollisions(ingameObject* object)
{
	for (auto& el : objects)
	{
		if (object->getID()!=el->getID())
		{
			if (checkCollisionWithObject(object, el.get()))
			{
				object->applyCollision(el.get());
			}
				
		}
	}
};

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
	particleEmitters.emplace_back(std::make_shared<ParticleEmitter>(importer, glm::vec3(0.0f,0.0f,-10.0f), 0.0, 5.0, 1000));
	for (int i=0;i<200; i++)
	{
		float size = (rand()%randCount)/1.5f + 1.5;
		glm::vec3 position = glm::vec3((rand()%randCount-randCount/2)/division,(rand()%randCount-randCount/2)/division,offset+ (rand()%randCount-randCount/2)/division);
		glm::vec3 speed= glm::vec3(0.0,0.0,0.0);
		glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

		objects.emplace_back(std::make_shared<Ball>(importer, size, position, speed,color));
	}


	// for (int i=0; i<1000;i++)
	// {
	// 	float size = (rand()%randCount)/10.0f + 0.3;
	// 	glm::vec3 position = glm::vec3((rand()%randCount-randCount/2)/division,(rand()%randCount-randCount/2)/division,offset+ (rand()%randCount-randCount/2)/division);
	// 	glm::vec3 speed = glm::vec3((1-2*rand()%2)*(rand()%randCount+1)/speedDiv, (1-2*rand()%2)*(rand()%randCount+1)/speedDiv,(1-2*rand()%2)*(rand()%randCount+1)/speedDiv);
	// 	glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

	// 	objects.emplace_back(std::make_shared<Ball>(importer, size, position, speed,color));
	// }
}
template<typename T>
void PhysicsModule::applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera)
{
	for (auto it = elements.begin(); it != elements.end(); )
    {
        if (!(*it)->isDeleted())
        {
			(*it)->resetForce();
			if (gravity)
				this->applyForceGrav(it->get());
			if (aero)
				this->applyForceAeroDyn(it->get());
			(*it)->checkCollisionWithDomain(centerOfDomain, borderOfDomain);
            applyCollisions(it->get());
			(*it)->process(fpsTime, shader, camera);
            ++it;
        }
        else
        {
            it = elements.erase(it);
        }
    }
}
void PhysicsModule::process(float fpsTime, Shaders* shaderProgram, Camera* camera)
{
	applyPhysicsToElements(objects, fpsTime, shaderProgram, camera);
	applyPhysicsToElements(particleEmitters,  fpsTime, shaderProgram, camera);
	for (auto it = particleEmitters.begin(); it != particleEmitters.end(); )
    {
		applyPhysicsToElements((*it)->particles,  fpsTime, shaderProgram, camera);
		++it;
    }
}
void PhysicsModule::addNewGravityCenter(glm::vec3 pos)
{
	gravityPoints.push_back(pos);
}