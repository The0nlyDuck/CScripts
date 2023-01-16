#pragma once
#include "RigidBody.h"

class Spring : public PhysicsObject
{
public:
	Spring(RigidBody* body1, RigidBody* body2,
		float restLength, float springCoefficient, float damping = 0.1f,
		glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));

	void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();

	glm::vec2 getContact1() { return m_body1 ? m_body1->ToWorld(m_contact1) : m_contact1; }
	glm::vec2 getContact2() { return m_body2 ? m_body2->ToWorld(m_contact2) : m_contact2; }
	void setContact1(glm::vec2 contact) { m_contact1 = contact; }
	void setContact2(glm::vec2 contact) { m_contact2 = contact; }

protected:
	RigidBody* m_body1;
	RigidBody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient; // the restoring force

};

