#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float angularVelocity, glm::vec4 colour) : RigidBody(ShapeType::SPHERE, position, velocity, 0, angularVelocity, mass)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * mass * radius * radius;
}

void Sphere::draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 50, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(0, 0, 0, 1));
}
