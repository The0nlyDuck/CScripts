#pragma once
#include "PhysicsObject.h"

#define MIN_LINEAR_THRESHOLD 0.01f
#define MIN_ANGULAR_THRESHOLD 0.01f

class RigidBody :  public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float angularVelocity, float mass);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force, glm::vec2 pos);
	virtual void resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual float getEnergy(glm::vec2 gravity);
	float getKineticEnergy();
	float getPotentialEnergy(glm::vec2 gravity);

	glm::vec2 getPosition() { return m_position; }
	float getOrientation() { return m_orientation; }

	glm::vec2 ToWorld(glm::vec2 localpos);
		 
	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	glm::vec2 getVelocity() { return m_velocity; }
	
	void setKinematic(bool state) { m_isKinematic = state; }
	bool isKinematic() { return m_isKinematic; }

	void setAngularVelocity(float velocity) { m_angularVelocity = velocity; }
	float getAngularVelocity() { return m_angularVelocity; }

	float getMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float getMoment() { return m_isKinematic ? INT_MAX : m_moment; }

	void setElasticity(float elasticity) { m_elasticity = elasticity; }
	float getElasticity() { return m_elasticity; }

	float getLineDrag() { return m_linearDrag; }
	float getAngleDrag() { return m_angularDrag; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	bool m_isKinematic = false;

	float m_mass;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;

	float m_elasticity = 1;

	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;

	// store the local x,y axes of the box based on its angle of rotation 
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

