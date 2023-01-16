#include "RigidBody.h"
#include "PhysicsScene.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float angularVelocity, float mass):PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_angularVelocity = angularVelocity;
	m_mass = mass;
}
RigidBody::~RigidBody()
{
}

//Newton's First Law
void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	
	if (length(m_velocity) < MIN_LINEAR_THRESHOLD) {
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) {
		m_angularVelocity = 0;
	}

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}

	m_position += m_velocity * timeStep * m_linearDrag;
	applyForce(gravity * m_mass * timeStep, glm::vec2{0,0});

	m_orientation += m_angularVelocity * timeStep * m_angularDrag;
}

//Newton's Second Law
void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	glm::vec2 acceleration = force / m_mass;
	m_velocity += acceleration;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / getMoment();
}

void RigidBody::resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	// find the vector between their centres, or use the provided direction of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal? *collisionNormal: actor2->getPosition() - m_position);
	// get the vector perpendicular to the collision normal 
	glm::vec2  perp(normal.y, -normal.x);

	// 'r' is the radius from axis to application of force 
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object  
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2 
	float v2 = glm::dot(actor2->m_velocity, normal) +
		r2 * actor2->m_angularVelocity;

	//Moving closer
	if(v1 > v2)
	{
		// calculate the effective mass at contact point for each object i.e. how much the contact point will move due to the force applied. 
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;
	
		this->applyForce(-force, contact - m_position); 
		actor2->applyForce(force, contact - actor2->m_position);
	}

}

float RigidBody::getEnergy(glm::vec2 gravity)
{
	return getKineticEnergy() + getPotentialEnergy(gravity);
}

float RigidBody::getKineticEnergy()
{
	
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

float RigidBody::getPotentialEnergy(glm::vec2 gravity)
{
	return -getMass() * glm::dot(gravity, getPosition());
}

glm::vec2 RigidBody::ToWorld(glm::vec2 localpos)
{
	return m_position + m_localX * localpos.x + m_localY * localpos.y;
}

