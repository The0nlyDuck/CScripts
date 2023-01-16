#include "Spring.h"

Spring::Spring(RigidBody* body1, RigidBody* body2,
	float restLength, float springCoefficient, float damping,
	glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(ShapeType::SPRING)
{
	m_body1 = body1;
	m_body2 = body2;
	m_contact1 = contact1;
	m_contact2 = contact2;
	m_restLength = restLength;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
}

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//get the world coordiantes of the end of the springs
	glm::vec2 p1 = getContact1();
	glm::vec2 p2 = getContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	//apply damping
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	//F = -kX - bv
	glm::vec2 force = (direction * m_springCoefficient * (m_restLength - length)) - (m_damping * relativeVelocity);
	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
}

void Spring::draw()
{
	aie::Gizmos::add2DLine(getContact1(), getContact2(), glm::vec4{ 1,1,1,1 });
}
