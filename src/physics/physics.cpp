#include "physics.h"

PhysicsModule::PhysicsModule(modelImporter* importer, Shaders* shaderProgram)
{
	christmasSetting(importer, shaderProgram);
}

void PhysicsModule::christmasSetting(modelImporter* importer, Shaders* shaderProgram)
{
	guiEnabled = false;
	float offset = -20.0f;
	float groundHeight = 0.01f;
	float coneHeight = 2.0f;
	float barkHeight = 0.5f;
	borderOfDomain = 8.0f;
	centerOfDomain = glm::vec3(0.0f,0.0f,offset);
	glUniform3f(glGetUniformLocation(shaderProgram->getID(), "lightPos"), centerOfDomain.x, centerOfDomain.y - borderOfDomain + 3.5f*coneHeight + 2*barkHeight + 2*groundHeight, centerOfDomain.z);

	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(borderOfDomain*100.0f ,groundHeight*100.0f,borderOfDomain*100.0f), glm::vec3(centerOfDomain.x,-borderOfDomain +centerOfDomain.y, centerOfDomain.z), glm::vec3(0.0f),glm::vec3(0.36f, 0.20f, 0.02f),true));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(barkHeight*100.0f , barkHeight*100.0f,barkHeight*100.0f), glm::vec3(centerOfDomain.x,centerOfDomain.y - borderOfDomain + 2*groundHeight + barkHeight, centerOfDomain.z), glm::vec3(0.0f),glm::vec3(0.36f, 0.20f, 0.02f),true));
	for (int i=0; i<3; i++)
		objects.emplace_back(std::make_shared<Cone>(importer, coneHeight*100, coneHeight*100,	glm::vec3(centerOfDomain.x,centerOfDomain.y - borderOfDomain + 2*barkHeight + 2*groundHeight + i*coneHeight/1.3f + coneHeight/2.0f,centerOfDomain.z),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.6f,0.1f), true));

	for (int i=-1; i<=1; i++)
	{
		for (int j=-1; j<=1; j++)
		{
			particleEmitters.emplace_back(std::make_shared<SnowEmitter>(importer, glm::vec3(centerOfDomain.x+i*2*borderOfDomain/3.0f,centerOfDomain.y+borderOfDomain*0.3f,centerOfDomain.z + 2*j*borderOfDomain/3.0f), 0.0, 5000, 10.0f));
		}
	}


}

void PhysicsModule::testingSetting(modelImporter* importer, Shaders* shaderProgram)
{
	guiEnabled = true;
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
		createRandomBall(importer, glm::vec3(0.0f,0.0f,offset), randCount, division);
	}
	createBoundingBox(importer, borderOfDomain, glm::vec3(0.0f,0.0f, offset));

	objects.emplace_back(std::make_shared<Cone>(importer, 120.0f, 240.0f,	glm::vec3(2.0f,0.0f,offset),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f)));
}




void PhysicsModule::createRandomBall(modelImporter* importer, glm::vec3 offset, int randCount, float division, glm::vec3 speed)
{
	float size = (rand()%randCount)/1.5f+1.5;
	glm::vec3 position = glm::vec3((offset.x + rand()%randCount-randCount/2)/division, offset.y + (rand()%randCount-randCount/2)/division,offset.z+ (rand()%randCount-randCount/2)/division);
	glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

	objects.emplace_back(std::make_shared<Ball>(importer, size, position, speed,color));
}

void PhysicsModule::createBoundingBox(modelImporter* importer, float borderOfDomain, glm::vec3 pos)
{
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(borderOfDomain*100.0f ,10.0f,borderOfDomain*100.0f), glm::vec3(pos.x,-borderOfDomain +pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f),true, false));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(borderOfDomain*100.0f,10.0f,borderOfDomain*100.0f), glm::vec3(pos.x,pos.y + borderOfDomain, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(10.0f,borderOfDomain*100.0f,borderOfDomain*100.0f), glm::vec3(borderOfDomain + pos.x,pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(10.0f,borderOfDomain*100.0f,borderOfDomain*100.0f), glm::vec3(-borderOfDomain+pos.x,pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(borderOfDomain*100.0f,borderOfDomain*100.0f,10.0f), glm::vec3(pos.x,pos.y, borderOfDomain+pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(borderOfDomain*100.0f,borderOfDomain*100.0f,10.0f), glm::vec3(pos.x,pos.y, -borderOfDomain+pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
}

float cellSize = 1.0f;

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

void PhysicsModule::checkCollisions(GameObject* o1, GameObject* o2)
{
	if (o1->getID()!=o2->getID())
		if ((o1->collidesWith(o2) || o2->collidesWith(o1) ) && (!o1->body.getIsStatic() || !o2->body.getIsStatic()))
		{
			if (o1->colliders->testCollision(&(o1->body), (o2->colliders).get(), &(o2->body)))
			{
				applyCollision(o1, o2);
			}
		}
};

void PhysicsModule::parseCollisionsNonGrid(GameObject* o1)
{
	for (auto& el : objects)
	{
		checkCollisions(o1, el.get());
	}
}

void PhysicsModule::applyCollision(GameObject* o1, GameObject* o2)
{

	float depth;
	glm::vec3 n; 
	glm::vec3 delta = o1->body.getPosition()-o2->body.getPosition();
	if (!o1->colliders->isConvex() && !o2->colliders->isConvex())
	{
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
		if (glm::dot(delta, n) < 0.0f)
			n = -n;
	}

	if (depth <= 0) return;
	float percent = 0.8f;
    float slop = 0.01f;
	float invMassA = o1->body.getIsStatic() ? 0.0f : 1.0f / o1->body.getMass();
	float invMassB = o2->body.getIsStatic() ? 0.0f : 1.0f / o2->body.getMass();

	float retvel = glm::dot(o1->body.getVelocity()-o2->body.getVelocity(),n);
    if (retvel > 0)
        return;

	float correctionMag = std::max(depth-slop, 0.0f)/(invMassA+invMassB)*percent;
    glm::vec3 correction = correctionMag*n;

	if (!o1->body.getIsStatic())
		o1->body.setPosition(o1->body.getPosition()+correction*invMassA);
	if (!o2->body.getIsStatic())
		o2->body.setPosition(o2->body.getPosition() -correction*invMassB);


	// Do pozniejszej implementacji
    float e = 0.0f;
    float j = -(1.0f+e)*retvel;
    j /= invMassA+invMassB;

    glm::vec3 impulse = j*n;
	
    o1->body.setVelocity(o1->body.getVelocity()+impulse*invMassA);
    o2->body.setVelocity(o2->body.getVelocity()-impulse*invMassB);

	// tarcie - do implementacji w osobnej metodzie
	glm::vec3 v_rel = o1->body.getVelocity() - o2->body.getVelocity();
	glm::vec3 v_normal = glm::dot(v_rel, n) * n;
	glm::vec3 v_tangent = v_rel - v_normal;

	float tLen = glm::length(v_tangent);
	if (tLen > 1e-6f)
		v_tangent /= tLen;
	else
		v_tangent = glm::vec3(0.0f);

	float jt = -glm::dot(v_rel, v_tangent);
	jt /= invMassA + invMassB;
	float mu = 0.3f;
	float maxFriction = j * mu;
	jt = glm::clamp(jt, -maxFriction, maxFriction);
	glm::vec3 frictionImpulse = jt * v_tangent;
	o1->body.setVelocity(o1->body.getVelocity() + frictionImpulse * invMassA);
	o2->body.setVelocity(o2->body.getVelocity() - frictionImpulse * invMassB);
}


void PhysicsModule::addElementToGrid(GameObject* o)
{
	glm::vec3 pos = o->body.getPosition();
	float size = o->getSize();
	CellKey keyMin = getCell(glm::vec3(pos.x-size, pos.y-size, pos.z-size));
	CellKey keyMax = getCell(glm::vec3(pos.x+size, pos.y+size, pos.z+size));
	o->primaryCell = getCell(glm::vec3(pos.x, pos.y, pos.z));
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
			if (gravity)
				this->applyForceGrav((it->get()));
			if (aero)
				this->applyForceAeroDyn(it->get());
			(*it)->process(fpsTime, shader, camera);
			addElementToGrid(it->get());
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
            parseCollisionsNonGrid(it->get());
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
	grid.clear();
	preprocessVector(objects, fpsTime, shaderProgram, camera);
	// applyPhysicsToElements(objects, fpsTime, shaderProgram, camera);
	applyPhysicsToElements(particleEmitters, fpsTime, shaderProgram, camera);
	for (auto it = particleEmitters.begin(); it != particleEmitters.end(); )
    {
		preprocessVector((*it)->particles, fpsTime, shaderProgram, camera);
		// applyPhysicsToElements((*it)->particles,  fpsTime, shaderProgram, camera);
		++it;
    }
	for (auto& [key, cell] : grid)
		for (size_t i = 0; i < cell.size(); ++i)
		{
			for (size_t j = i+1; j < cell.size(); ++j)
			{
				if (key != cell[i]->primaryCell && key != cell[j]->primaryCell)
					continue;
				checkCollisions(cell[i], cell[j]);
			}
		}

				

}
void PhysicsModule::addNewGravityCenter(glm::vec3 pos)
{
	gravityPoints.push_back(pos);
}