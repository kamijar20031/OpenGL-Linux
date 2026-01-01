#include "colliders.h"


struct Simplex {
    std::vector<glm::vec3> pts;
};

bool SimplexLine(Simplex& s, glm::vec3& dir)
{
	glm::vec3 A = s.pts[1];
	glm::vec3 B = s.pts[0];

	glm::vec3 AB = B-A;
	glm::vec3 A0 = -A;

	if (glm::dot(AB, A0) > 0.0f)
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

bool SimplexTriangle(Simplex& s, glm::vec3& dir)
{
	glm::vec3 A = s.pts[2];
    glm::vec3 B = s.pts[1];
    glm::vec3 C = s.pts[0];
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 A0 = -A;
    glm::vec3 ABC = glm::cross(AB, AC);
	if (glm::dot(glm::cross(ABC, AC),A0)> 0.0f)
	{
		if (glm::dot(AC,A0) > 0.0f)
		{
			s.pts = {C,A}; 
			dir = glm::cross(glm::cross(AC,A0), AC);
		}
		else
		{	
			s.pts = {B,A}; 
			return SimplexLine(s, dir);
		}
	}
	else
	{
		if (glm::dot(glm::cross(AB, ABC),A0) > 0.0f)
		{
			s.pts = {B,A}; 
			return SimplexLine(s, dir);
		}
		else
		{	
			if (glm::dot(ABC, A0) > 0.0f)
			{
				dir = ABC;
			}
			else
			{
				s.pts = {B, C, A}; 
				dir = -ABC;
			}

		}
	}
	return false;
}

bool SimplexTetrahedron(Simplex& s, glm::vec3& dir)
{
    glm::vec3 A = s.pts[3];
	glm::vec3 B = s.pts[2];
	glm::vec3 C = s.pts[1];
	glm::vec3 D = s.pts[0];
	
	glm::vec3 AB = B -A;
	glm::vec3 AC = C -A;
	glm::vec3 AD = D -A;
	glm::vec3 A0 = -A;

	glm::vec3 ABC = glm::cross(AB, AC);
	glm::vec3 ACD = glm::cross(AC, AD);
	glm::vec3 ADB = glm::cross(AD, AB);

	if (glm::dot(ABC, A0) > 0.0f)
	{
		s.pts = {C, B, A};
		return SimplexTriangle(s, dir);
	}
	if (glm::dot(ACD, A0) > 0.0f)
	{
		s.pts = {D, C, A};
		return SimplexTriangle(s, dir);
	}
	if (glm::dot(ADB, A0) > 0.0f)
	{
		s.pts = {B, D, A};
		return SimplexTriangle(s, dir);
	}

	return true;

}

bool handleSimplex(Simplex& s, glm::vec3& dir)
{
	if (s.pts.size()==2)
	{
		return SimplexLine(s, dir);
	}
	else if (s.pts.size() == 3)
	{
		return SimplexTriangle(s, dir);
	}
	else if (s.pts.size() == 4) 
	{
		return SimplexTetrahedron(s, dir);
    }

    return false;
}

bool gjkIntersect(ConvexCollider* a, glm::vec3 posA, Collider* b, glm::vec3 posB)
{
	auto support = [&](glm::vec3 dir) {return a->support(dir)+ posA - (b->support(-dir) + posB);};
	glm::vec3 dir = glm::vec3(1.0f,0.0f,0.0f);
	Simplex simplex;
	glm::vec3 sup = support(dir);
	simplex.pts.push_back(sup);
	dir = -sup;
    for (int i =0; i<15; i++) 
	{
        sup = support(dir);
        if (glm::dot(sup, dir) < 1e-6f)
            return false;
        simplex.pts.push_back(sup);
        if (handleSimplex(simplex, dir))
		{
			a->setLastDirection(dir);
			return true;
		}
            
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
	return glm::normalize(d)*radius;
}