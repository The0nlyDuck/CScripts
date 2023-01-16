#pragma once
#include "RigidBody.h"
class Box : public RigidBody
{ 
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, float orientaion, float angularVelocity, glm::vec4 colour);
	~Box(){}

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void draw();

	glm::vec2 getExtents() { return m_extents; }

	float getWidth() { return m_extents.x * 2; }
	float getHeight() { return m_extents.y * 2; }

	glm::vec2 getXLocal() { return m_localX; }
	glm::vec2 getYLocal() { return m_localY; }

	bool checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

private:
	glm::vec2 m_extents;   // the halfedge lengths  
	glm::vec4 m_colour;
};

