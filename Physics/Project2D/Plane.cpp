#include "Plane.h"
Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
	m_colour = glm::vec4(0, 1, 0, 1);
}
Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
}
void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}
void Plane::draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z 
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour); 
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour,	colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_colour, colourFade, colourFade);
}

void Plane::resolveCollision(RigidBody* actor2, glm::vec2 contact)
{
	// the position at which we'll apply the force relative to the object's COM 
	glm::vec2 localContact = contact - actor2->getPosition();

	glm::vec2 normal = m_normal;

	// the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	glm::vec2 vRel = actor2->getVelocity() + actor2->getAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, normal);

	//elasticity for collisions
	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F * r
	float r = glm::dot(localContact, glm::vec2(normal.y, -normal.x));

	// work out the "effective mass" - this is a combination of moment of 
	// inertia and mass, and tells us how much the contact point velocity  
	// will change with the force we're applying 
	float mass0 = 1.0f / (1.0f / actor2->getMass() + (r * r) / actor2->getMoment());

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;

	glm::vec2 force = normal * j;

	actor2->applyForce(force, contact - actor2->getPosition());
}
