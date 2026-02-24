#include "physics.h"


float PhysicsModule::stiffness = 0.7f;
float PhysicsModule::mu = -3.0f;
bool PhysicsModule::gravity = false;
bool PhysicsModule::aero = false;
bool PhysicsModule::guiEnabled = true;
float PhysicsModule::borderOfDomain = 8.0f;
float PhysicsModule::g = 9.81f;
glm::vec3 PhysicsModule::centerOfDomain;
std::vector <glm::vec3> PhysicsModule::gravityPoints;

std::vector <std::shared_ptr<ParticleEmitter>> PhysicsModule::particleEmitters = {};
std::vector <std::shared_ptr<GameObject>> PhysicsModule::objects = {};
std::vector <std::shared_ptr<SoftBody>> PhysicsModule::softBodies = {};
std::unordered_map<CellKey, std::vector<GameObject*>, CellKeyHash> PhysicsModule::grid;

void PhysicsModule::init(modelImporter* importer, Shaders* shaderProgram)
{
	ThreadPool::start(36);
	// christmasSetting(importer, shaderProgram);
	// PhysicsModule::testingSetting(importer, shaderProgram);
	PhysicsModule::softBodyTestSetting(importer, shaderProgram);
}

void PhysicsModule::christmasSetting(modelImporter* importer, Shaders* shaderProgram)
{
	PhysicsModule::guiEnabled = false;
	float offset = -20.0f;
	float groundHeight = 0.01f;
	float coneHeight = 2.0f;
	float barkHeight = 0.5f;
	PhysicsModule::centerOfDomain = glm::vec3(0.0f,0.0f,offset);
	glUniform3f(glGetUniformLocation(shaderProgram->getID(), "lightPos"), PhysicsModule::centerOfDomain.x, PhysicsModule::centerOfDomain.y - PhysicsModule::borderOfDomain + 3.5f*coneHeight + 2*barkHeight + 2*groundHeight, PhysicsModule::centerOfDomain.z);

	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(PhysicsModule::borderOfDomain*100.0f ,groundHeight*100.0f,PhysicsModule::borderOfDomain*100.0f), glm::vec3(PhysicsModule::centerOfDomain.x,-PhysicsModule::borderOfDomain +PhysicsModule::centerOfDomain.y, PhysicsModule::centerOfDomain.z), glm::vec3(0.0f),glm::vec3(0.36f, 0.20f, 0.02f),true));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(barkHeight*100.0f , barkHeight*100.0f,barkHeight*100.0f), glm::vec3(PhysicsModule::centerOfDomain.x,PhysicsModule::centerOfDomain.y - PhysicsModule::borderOfDomain + 2*groundHeight + barkHeight, PhysicsModule::centerOfDomain.z), glm::vec3(0.0f),glm::vec3(0.36f, 0.20f, 0.02f),true));
	for (int i=0; i<3; i++)
		PhysicsModule::objects.emplace_back(std::make_shared<Cone>(importer, coneHeight*100, coneHeight*100,	glm::vec3(PhysicsModule::centerOfDomain.x,PhysicsModule::centerOfDomain.y - PhysicsModule::borderOfDomain + 2*barkHeight + 2*groundHeight + i*coneHeight/1.3f + coneHeight/2.0f,PhysicsModule::centerOfDomain.z),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.6f,0.1f), true));

	for (int i=-1; i<=1; i++)
	{
		for (int j=-1; j<=1; j++)
		{
			PhysicsModule::particleEmitters.emplace_back(std::make_shared<SnowEmitter>(importer, glm::vec3(PhysicsModule::centerOfDomain.x+i*2*PhysicsModule::borderOfDomain/3.0f,PhysicsModule::centerOfDomain.y+PhysicsModule::borderOfDomain*0.3f,PhysicsModule::centerOfDomain.z + 2*j*PhysicsModule::borderOfDomain/3.0f), 0.0));

			glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);
			PhysicsModule::particleEmitters.emplace_back(std::make_shared<FireworkEmitter>(importer, glm::vec3(PhysicsModule::centerOfDomain.x+i*2*PhysicsModule::borderOfDomain/3.0f,PhysicsModule::centerOfDomain.y-PhysicsModule::borderOfDomain*0.2f,PhysicsModule::centerOfDomain.z + 2*j*PhysicsModule::borderOfDomain/3.0f), color));
		}
	}


}

void PhysicsModule::testingSetting(modelImporter* importer, Shaders* shaderProgram)
{
    int randCount = 50;
	float offset = -10.0f;
	PhysicsModule::borderOfDomain = 4.0f;
	PhysicsModule::centerOfDomain = glm::vec3(0.0f,0.0f,offset);
	float division = (float)(randCount/2)/(PhysicsModule::borderOfDomain-0.1f);
	float speedDiv = 45.0f;
	glUniform3f(glGetUniformLocation(shaderProgram->getID(), "lightPos"), 0.0f, 0.0f, offset);
	PhysicsModule::gravityPoints.push_back(glm::vec3(PhysicsModule::centerOfDomain));
	PhysicsModule::particleEmitters.emplace_back(std::make_shared<ParticleEmitter>(importer, glm::vec3(0.0f,0.0f,-10.0f), 0.0, 5.0, 1000));
	for (int i=0;i<200; i++)
	{
		PhysicsModule::createRandomBall(importer, glm::vec3(0.0f,0.0f,offset), randCount, division);
	}
	PhysicsModule::createBoundingBox(importer, glm::vec3(0.0f,0.0f, offset));

	PhysicsModule::objects.emplace_back(std::make_shared<Cone>(importer, 120.0f, 240.0f,	glm::vec3(2.0f,0.0f,offset),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f)));
}

void PhysicsModule::softBodyTestSetting(modelImporter* importer, Shaders* shaderProgram)
{
	glm::vec3 halfExtent = glm::vec3(1.0f,2.0f,1.0f);
	float groundHeight = 0.01f;
	float offset = -10.0f;
	PhysicsModule::centerOfDomain = glm::vec3(0.0f,0.0f,offset);
	glUniform3f(glGetUniformLocation(shaderProgram->getID(), "lightPos"), PhysicsModule::centerOfDomain.x, PhysicsModule::centerOfDomain.y, PhysicsModule::centerOfDomain.z);

	PhysicsModule::softBodies.emplace_back(std::make_shared<SoftRectangular>(importer, glm::vec3(PhysicsModule::centerOfDomain.x, PhysicsModule::centerOfDomain.y-PhysicsModule::borderOfDomain + groundHeight*4.0f + halfExtent.y, PhysicsModule::centerOfDomain.z), halfExtent,10, glm::vec3(1.0f,0.0f,0.0f)));
	// PhysicsModule::softBodies.emplace_back(std::make_shared<SoftRectangular>(importer, glm::vec3(PhysicsModule::centerOfDomain.x, PhysicsModule::centerOfDomain.y - groundHeight*4.0f + halfExtent.y, PhysicsModule::centerOfDomain.z), halfExtent,6,  glm::vec3(1.0f,0.0f,0.0f)));

}

void PhysicsModule::createRandomBall(modelImporter* importer, glm::vec3 offset, int randCount, float division, glm::vec3 speed)
{
	float size = (rand()%randCount)/1.5f+1.5;
	glm::vec3 position = glm::vec3((offset.x + rand()%randCount-randCount/2)/division, offset.y + (rand()%randCount-randCount/2)/division,offset.z+ (rand()%randCount-randCount/2)/division);
	glm::vec3 color = glm::vec3((rand()%255)/255.0f, (rand()%255)/255.0f,(rand()%255)/255.0f);

	PhysicsModule::objects.emplace_back(std::make_shared<Ball>(importer, size, position, speed,color));
}

void PhysicsModule::createBoundingBox(modelImporter* importer, glm::vec3 pos)
{
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(PhysicsModule::borderOfDomain*100.0f ,10.0f,PhysicsModule::borderOfDomain*100.0f), glm::vec3(pos.x,-PhysicsModule::borderOfDomain +pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f),true, false));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(PhysicsModule::borderOfDomain*100.0f,10.0f,PhysicsModule::borderOfDomain*100.0f), glm::vec3(pos.x,pos.y + PhysicsModule::borderOfDomain, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(10.0f,PhysicsModule::borderOfDomain*100.0f,PhysicsModule::borderOfDomain*100.0f), glm::vec3(PhysicsModule::borderOfDomain + pos.x,pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(10.0f,PhysicsModule::borderOfDomain*100.0f,PhysicsModule::borderOfDomain*100.0f), glm::vec3(-PhysicsModule::borderOfDomain+pos.x,pos.y, pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(PhysicsModule::borderOfDomain*100.0f,PhysicsModule::borderOfDomain*100.0f,10.0f), glm::vec3(pos.x,pos.y, PhysicsModule::borderOfDomain+pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
	PhysicsModule::objects.emplace_back(std::make_shared<Rectangular>(importer, glm::vec3(PhysicsModule::borderOfDomain*100.0f,PhysicsModule::borderOfDomain*100.0f,10.0f), glm::vec3(pos.x,pos.y, -PhysicsModule::borderOfDomain+pos.z), glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f), true, false));
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
	PhysicsModule::applyNonPointGravity(object);
	for (auto& i : PhysicsModule::gravityPoints)
	{
		glm::vec3 mag3 = object->body.getPosition()-i;
		float magL = glm::length(mag3);
		object->body.applyForce(-mag3/(float)(pow(magL+epsilon, 3.0f)));
	}
		
}

void PhysicsModule::applyNonPointGravity(GameObject* object)
{
	object->body.applyForce(-g*glm::vec3(0.0f,1.0f,0.0f)*object->body.getMass());

}

void PhysicsModule::applyForceAeroDyn(GameObject* object)
{
    object->body.applyForce(-6.0f*object->body.getVelocity()*(float)(pow(10,PhysicsModule::mu))*object->getSize()*(float)(std::numbers::pi));
}

void PhysicsModule::applyElasticForceForSoftBody(SoftBody* body)
{
	body->processElasticForces(PhysicsModule::stiffness);
}

void PhysicsModule::checkCollisions(GameObject* o1, GameObject* o2)
{
	if (o1->getID()==o2->getID())
		return;

	if (((o1->collisionLayer & o2->collisionMask) == 0 || (o2->collisionLayer & o1->collisionMask) == 0))
		return;

	if ((o1->body.getIsStatic() && o2->body.getIsStatic()))
		return;
		
	if (o1->colliders->testCollision(&(o1->body), (o2->colliders).get(), &(o2->body)))
			PhysicsModule::applyCollision(o1, o2);
		
};

void PhysicsModule::parseCollisionsNonGrid(GameObject* o1)
{
	for (auto& el : PhysicsModule::objects)
	{
		PhysicsModule::checkCollisions(o1, el.get());
	}
}

void PhysicsModule::applyFrictionOnCollision(GameObject* o1, GameObject* o2, glm::vec3 n, float j, float invMassA, float invMassB)
{
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
	float mu = std::max(o1->body.getFriction(), o2->body.getFriction());
	float maxFriction = j * mu;
	jt = glm::clamp(jt, -maxFriction, maxFriction);
	glm::vec3 frictionImpulse = jt * v_tangent;
	if (!o1->body.getIsStatic())
		o1->body.setVelocity(o1->body.getVelocity() + frictionImpulse * invMassA);
	if (!o2->body.getIsStatic())	
		o2->body.setVelocity(o2->body.getVelocity() - frictionImpulse * invMassB);
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

    float e = std::min(o1->body.getElasticity(), o2->body.getElasticity());
    float j = -(1.0f+e)*retvel;
    j /= invMassA+invMassB;

    glm::vec3 impulse = j*n;
	
	if (!o1->body.getIsStatic())
    	o1->body.setVelocity(o1->body.getVelocity()+impulse*invMassA);
	if (!o2->body.getIsStatic())
    	o2->body.setVelocity(o2->body.getVelocity()-impulse*invMassB);

	PhysicsModule::applyFrictionOnCollision(o1,o2,n,j,invMassA, invMassB);
	
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
				PhysicsModule::grid[temp].push_back(o);
			}
		}
	}
}
template<typename T>
void PhysicsModule::preprocessVector(std::vector<std::shared_ptr<T>>& elements,  float fpsTime, Shaders* shader, Camera* camera)
{
	for (auto it = elements.begin(); it != elements.end(); )
	{
		if (!(*it)->isDeleted())
		{
			if (PhysicsModule::gravity)
				PhysicsModule::applyForceGrav((it->get()));
			if (PhysicsModule::aero)
				PhysicsModule::applyForceAeroDyn(it->get());
			PhysicsModule::addElementToGrid(it->get());
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
			if (PhysicsModule::gravity)
				PhysicsModule::applyForceGrav(it->get());
			if (PhysicsModule::aero)
				PhysicsModule::applyForceAeroDyn(it->get());
            PhysicsModule::parseCollisionsNonGrid(it->get());
			(*it)->process(fpsTime, shader, camera);
            ++it;
        }
        else
        {
            it = elements.erase(it);
        }
    }
}

uint64_t makePairID(GameObject* a, GameObject* b)
{
	
    uint32_t idA = a->getID();
    uint32_t idB = b->getID();

    uint32_t minID = std::min(idA, idB);
    uint32_t maxID = std::max(idA, idB);

    return (uint64_t(minID) << 32) | maxID;
}



void PhysicsModule::process(float fpsTime, Shaders* shaderProgram, Camera* camera)
{
	PhysicsModule::grid.clear();
	PhysicsModule::preprocessVector(PhysicsModule::objects, fpsTime, shaderProgram, camera);
	PhysicsModule::applyPhysicsToElements(PhysicsModule::particleEmitters, fpsTime, shaderProgram, camera);
	for (auto i : PhysicsModule::particleEmitters)
		PhysicsModule::preprocessVector(i->particles, fpsTime, shaderProgram, camera);


	for (auto body : PhysicsModule::softBodies)
	{
		PhysicsModule::applyElasticForceForSoftBody(body.get());
		PhysicsModule::preprocessVector(body->vertices, fpsTime, shaderProgram, camera);
	}
		

	std::unordered_set<uint64_t> checked;

	for (auto& [key, cell] : PhysicsModule::grid)
		for (size_t i = 0; i < cell.size(); ++i)
		{
			for (size_t j = i+1; j < cell.size(); ++j)
			{
				uint64_t key = makePairID(cell[i], cell[j]);
				if (checked.insert(key).second)
				{
					checkCollisions(cell[i], cell[j]);
					
				}
			}
		}


}
void PhysicsModule::addNewGravityCenter(glm::vec3 pos)
{
	PhysicsModule::gravityPoints.push_back(pos);
}