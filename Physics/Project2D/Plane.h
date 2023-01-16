#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance, glm::vec4 colour);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();

	void resolveCollision(RigidBody* actor2, glm::vec2 contact);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_elasticity = 1;
	float m_distanceToOrigin;
	glm::vec4 m_colour;
};

