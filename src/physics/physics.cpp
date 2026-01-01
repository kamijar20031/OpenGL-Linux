#include "physics.h"

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
		float size = (rand()%randCount)/1.5f+1.5;
		glm::vec3 position = glm::vec3((rand()%randCount-randCount/2)/division,(rand()%randCount-randCount/2)/division,offset+ (rand()%randCount-randCount/2)/division);
		glm::vec3 speed= glm::vec3(0.0,0.0,0.0);
		glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

		objects.emplace_back(std::make_shared<Ball>(importer, size, position, speed,color));
	}
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(200.0f,10.0f,200.0f), glm::vec3(0.0f,-1.0f, offset), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f)));

}




float cellSize = 1000000000000.0f;

CellKey getCell(glm::vec3 pos)
{
	return {
		(int)std::floor(pos.x/cellSize),
		(int)std::floor(pos.y/cellSize),
		(int)std::floor(pos.z/cellSize)
	};
}


void PhysicsModule::applyForceGrav(GameObject* object)
{
    
    float epsilon = 0.2f;
	for (auto& i : gravityPoints)
	{
		glm::vec3 mag3 = object->body.getPosition()-i;
		float magL = glm::length(mag3);
		object->body.applyForce(-mag3/(float)(pow(magL+epsilon, 3.0f)));
	}
		
}

void PhysicsModule::applyForceAeroDyn(GameObject* object)
{
    object->body.applyForce(-6.0f*object->body.getVelocity()*(float)(pow(10,mu))*object->getSize()*(float)(std::numbers::pi));
}

void PhysicsModule::applyCollisions(GameObject* o1, GameObject* o2)
{
	for (auto& el : objects)
	{
		if (o1->getID()!=el->getID())
		{
			if (o1->colliders->testCollision(&(o1->body), (el->colliders).get(), &(el->body)))
			{
				applyCollision(o1, el.get());
			}
				
		}
	}
	// if (o1->collidesWith(o2))
	// {
	// 	if (o1->colliders->testCollision(&(o1->body), (o2->colliders).get(), &(o2->body)))
	// 	{
	// 		applyCollision(o1, o2);
	// 	}
	// }
};

void PhysicsModule::simpleCollision(GameObject* o1, GameObject* o2)
{
	glm::vec3 n;
	glm::vec3 delta = o1->body.getPosition()-o2->body.getPosition();
	float len2 = glm::dot(delta, delta);
	if (len2 < 1e-6f)
		return;
	n = delta / (float)sqrt(len2);

	float v1n = glm::dot(o1->body.getVelocity(), n);
	float v2n = glm::dot(o2->body.getVelocity(), n);

	float relVel = glm::dot(o1->body.getVelocity()-o2->body.getVelocity(), n);
	if (relVel>0)
		return;
	glm::vec3 v1n_vec = v1n*n;
	glm::vec3 v2n_vec = v2n*n;

	glm::vec3 v1t = o1->body.getVelocity()-v1n_vec;
	glm::vec3 v2t = o2->body.getVelocity()-v2n_vec;

	float v1ni = ( (o1->body.getMass()-o2->body.getMass())*v1n+2*o2->body.getMass()*v2n ) / (o1->body.getMass()+o2->body.getMass());
	float v2ni = ( (o2->body.getMass()-o1->body.getMass())*v2n+2*o1->body.getMass()*v1n ) / (o1->body.getMass()+o2->body.getMass());

	o1->body.setVelocity(v1t+v1ni*n);
	o2->body.setVelocity(v2t+v2ni*n);
}


void PhysicsModule::applyCollision(GameObject* o1, GameObject* o2)
{

	float depth;
	glm::vec3 n; 
	if (!o1->colliders->isConvex() && !o2->colliders->isConvex())
	{
		glm::vec3 delta = o1->body.getPosition()-o2->body.getPosition();
		float len2 = glm::dot(delta, delta);
		if (len2 < 1e-6f)
			return;
		n = delta / (float)sqrt(len2);
		depth = o1->getSize()+o2->getSize()-(float)sqrt(len2);
	}
	else
	{
		if (!o1->colliders->isConvex())
		{
			n = o2->colliders->resultingEPA.normal;
			depth = o2->colliders->resultingEPA.depth;
		}
		else
		{
			n = o1->colliders->resultingEPA.normal;
			depth = o1->colliders->resultingEPA.depth;
		}
	}

	if (depth <= 0) return;
	float percent = 0.8f;
    float slop = 0.01f;
    float invMassA = 1.0f/o1->body.getMass();
    float invMassB = 1.0f/o2->body.getMass();

	float correctionMag = std::max(depth-slop, 0.0f)/(invMassA+invMassB)*percent;
    glm::vec3 correction = correctionMag*n;

    o1->body.setPosition(o1->body.getPosition()+correction*invMassA);
    o2->body.setPosition(o2->body.getPosition() -correction*invMassB);

	float retvel = glm::dot(o1->body.getVelocity()-o2->body.getVelocity(),n);
    if (retvel > 0)
        return;

	// Do pozniejszej implementacji
    float e = 1.0f;
    float j = -(1.0f+e)*retvel;
    j /= invMassA+invMassB;

    glm::vec3 impulse = j*n;
	
    o1->body.setVelocity(o1->body.getVelocity()+impulse*invMassA);
    o2->body.setVelocity(o2->body.getVelocity()-impulse*invMassB);
}


void PhysicsModule::addElementToGrid(GameObject* o)
{
	glm::vec3 pos = o->body.getPosition();
	float size = o->getSize();
	CellKey keyMin = getCell(glm::vec3(pos.x-size, pos.y-size, pos.z-size));
	CellKey keyMax = getCell(glm::vec3(pos.x+size, pos.y+size, pos.z+size));
	for (int i =std::get<0>(keyMin); i<=std::get<0>(keyMax);i++)
	{
		for (int j =std::get<1>(keyMin); j<=std::get<1>(keyMax);j++)
		{
			for (int k=std::get<2>(keyMin); k<=std::get<2>(keyMax); k++)
			{
				CellKey temp{i, j, k};
				grid[temp].push_back(o);
			}
		}
	}
}
template<typename T>
void PhysicsModule::preprocessVector(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera)
{
	for (auto it = elements.begin(); it != elements.end(); )
	{
		if (!(*it)->isDeleted())
		{
			addElementToGrid(it->get());
			if (gravity)
				this->applyForceGrav(it->get());
			if (aero)
				this->applyForceAeroDyn(it->get());
			for (auto &el : objects)
			{
				applyCollisions(it->get(), el.get());
			}
			(*it)->process(fpsTime, shader, camera);
			++it;
		}
		else
		{
			it = elements.erase(it);
		}
	}
}

template<typename T>
void PhysicsModule::applyPhysicsToElements(std::vector<std::shared_ptr<T>>& elements, float fpsTime, Shaders* shader, Camera* camera)
{
	for (auto it = elements.begin(); it != elements.end(); )
    {
        if (!(*it)->isDeleted())
        {
			(*it)->body.resetForce();
			if (gravity)
				this->applyForceGrav(it->get());
			if (aero)
				this->applyForceAeroDyn(it->get());
            applyCollisions(it->get(), it->get());
			(*it)->process(fpsTime, shader, camera);
            ++it;
        }
        else
        {
            it = elements.erase(it);
        }
    }
}

void PhysicsModule::refreshGrid(float fpsTime, Shaders* shader, Camera* camera)
{
	grid.clear();
	preprocessVector(particleEmitters, fpsTime, shader, camera);
	preprocessVector(objects, fpsTime, shader, camera);

	for (auto it = particleEmitters.begin(); it != particleEmitters.end(); )
    {
		preprocessVector((*it)->particles, fpsTime, shader, camera);
		it++;
    }

}

void PhysicsModule::process(float fpsTime, Shaders* shaderProgram, Camera* camera)
{

	applyPhysicsToElements(objects, fpsTime, shaderProgram, camera);
	applyPhysicsToElements(particleEmitters, fpsTime, shaderProgram, camera);
	for (auto it = particleEmitters.begin(); it != particleEmitters.end(); )
    {
		applyPhysicsToElements((*it)->particles,  fpsTime, shaderProgram, camera);
		++it;
    }
	// this->refreshGrid(fpsTime, shaderProgram, camera);
	// for (auto& [key, cell] : grid)
	// 	for (size_t i = 0; i < cell.size(); ++i)
	// 		for (size_t j = i+1; j < cell.size(); ++j)
	// 			applyCollisions(cell[i], cell[j]);

}
void PhysicsModule::addNewGravityCenter(glm::vec3 pos)
{
	gravityPoints.push_back(pos);
}