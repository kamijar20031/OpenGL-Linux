#include "colliders.h"

struct Simplex {
    std::vector<glm::vec3> pts;
};

bool handleSimplex(Simplex& s, glm::vec3& dir)
{
	if (s.pts.size()==2)
	{
		glm::vec3 A = s.pts[1];
		glm::vec3 B = s.pts[0];

		glm::vec3 AB = B-A;
		glm::vec3 A0 = -A;

		if (glm::dot(AB, A0) > 0)
		{
			dir = glm::cross(glm::cross(AB, A0),AB);
		}
		else
		{
			s.pts = {A};
			dir = A0;
		}
		return false;
	}
	else if (s.pts.size() == 3)
	{
		glm::vec3 A = s.pts[2];
        glm::vec3 B = s.pts[1];
        glm::vec3 C = s.pts[0];
        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 AO = -A;
        glm::vec3 ABC = glm::cross(AB, AC);
        glm::vec3 ABperp = glm::cross(glm::cross(AC, AB), AB);
        if (glm::dot(ABperp, AO) > 0) {
            s.pts = { A, B };
            dir = ABperp;
            return false;
        }
        glm::vec3 ACperp = glm::cross(glm::cross(AB, AC), AC);
        if (glm::dot(ACperp, AO) > 0) {
            s.pts = { A, C };
            dir = ACperp;
            return false;
        }
        dir = (glm::dot(ABC, AO) > 0 ? ABC : -ABC);
        return false;
	}
	else if (s.pts.size() == 4) 
	{
        glm::vec3 A = s.pts[3];
		glm::vec3 B = s.pts[2];
		glm::vec3 C = s.pts[1];
		glm::vec3 D = s.pts[0];
		glm::vec3 AO = -A;
		glm::vec3 ABC = glm::cross(B-A, C-A);
		if (glm::dot(ABC, AO) > 0) 
		{ 
			s.pts = {A,B,C}; 
			dir = ABC; 
			return false; 
		}
		glm::vec3 ACD = glm::cross(C-A, D-A);
		if (glm::dot(ACD, AO) > 0) 
		{ 
			s.pts = {A,C,D}; 
			dir = ACD; 
			return false; 
		}
		glm::vec3 ADB = glm::cross(D-A, B-A);
		if (glm::dot(ADB, AO) > 0) 
		{ 
			s.pts = {A,D,B}; 
			dir = ADB; 
			return false; 
		}
		return true;
    }

    return false;
}

bool gjkIntersect(ConvexCollider* a, glm::vec3 posA, Collider* b, glm::vec3 posB)
{
	auto support = [&](glm::vec3 dir) {return a->support(dir)+ posA - (b->support(-dir) + posB);};
	glm::vec3 dir = glm::vec3(1.0f,0.0f,0.0f);
	Simplex simplex;
	glm::vec3 A = support(dir);
	dir = -A;
	simplex.pts.push_back(A);
    for (int i = 0; i < 10; ++i) {
        glm::vec3 p = support(dir);
        if (glm::dot(p, dir) <= 0)
            return false;
        simplex.pts.push_back(p);
		a->setLastDirection(dir);
        if (handleSimplex(simplex, dir))
            return true;
    }
    return false;

}

Collider::~Collider() = default;

bool SphereCollider::testCollision( PhysicsBody* a,  Collider* b,  PhysicsBody* bBody) 
{
    return b->collidedWithSphere(bBody, this, a);
}
bool ConvexCollider::testCollision( PhysicsBody* a,  Collider* b,  PhysicsBody* bBody) 
{
    return b->collidedWithConvex(bBody, this, a);
}

// Kolizje z poszczegolnymi obiektami

bool SphereCollider::collidedWithSphere( PhysicsBody* a,  SphereCollider* b,  PhysicsBody* bBody) 
{
    float r = this->getRadius() + b->getRadius();
	glm::vec3 d = (a->getPosition() + this->getLocalOffset()) - (bBody->getPosition() + b->getLocalOffset());
	float dist2 = glm::dot(d, d);
	return dist2 <= r * r;	
}
// Do zaimplementowania
bool ConvexCollider::collidedWithSphere( PhysicsBody* a,  SphereCollider* b,  PhysicsBody* bBody) 
{
	return gjkIntersect(this, a->getPosition(), b, bBody->getPosition());
}
bool SphereCollider::collidedWithConvex( PhysicsBody* a,  ConvexCollider* b,  PhysicsBody* bBody) 
{
	return gjkIntersect(b, bBody->getPosition(), this, a->getPosition());
}

bool ConvexCollider::collidedWithConvex( PhysicsBody* a,  ConvexCollider* b,  PhysicsBody* bBody) 
{
	return gjkIntersect(this, a->getPosition(), b, bBody->getPosition());
}

glm::vec3 ConvexCollider::support(glm::vec3 d) 
{
	switch (type)
	{
		case ConvexType::Box:
		{
			return glm::vec3(
				d.x >= 0.0f ? values.x : -values.x,
				d.y >= 0.0f ? values.y : -values.y,
				d.z >= 0.0f ? values.z : -values.z
			);
		}

		case ConvexType::Cone: 
		{
			glm::vec3 apex = glm::vec3(0.0f, values.y * 0.5f, 0.0f);
			glm::vec3 base = glm::vec3(0.0f, -values.y * 0.5f, 0.0f);
			glm::vec3 dirXZ(d.x, 0.0f, d.z);
			if (glm::length(dirXZ) < 1e-6f) dirXZ = glm::vec3(1,0,0);
			dirXZ = glm::normalize(dirXZ);
			glm::vec3 basePoint = base + dirXZ * values.x;
			return glm::dot(apex, d) > glm::dot(basePoint, d)
				? apex : basePoint;
		}
		case ConvexType::Capsule: 
		{
			glm::vec3 dir = glm::normalize(d);
			glm::vec3 a = glm::vec3(0, values.y * 0.5f, 0);
			glm::vec3 b = glm::vec3(0, -values.y * 0.5f, 0);
			glm::vec3 dirNorm = glm::length(d) > 0.0f ? glm::normalize(d) : glm::vec3(0,1,0);
			glm::vec3 p = glm::dot(a, dirNorm) > glm::dot(b, dirNorm) ? a : b;
			return p + dirNorm * values.x;
		}
	}
	return glm::vec3(0.0f);
	
}

glm::vec3 SphereCollider::support(glm::vec3 d) 
{
	glm::vec3 n = glm::length(d) >0 ? glm::normalize(d) : glm::vec3(0.0f,1.0f,0.0f);

	return center + n*radius;
}