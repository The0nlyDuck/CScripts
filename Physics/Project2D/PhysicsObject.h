#pragma once
#include <iostream>
#include "Renderer2D.h"
#include "Gizmos.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"


enum class ShapeType {
	SPRING = -1,
	PLANE,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	ShapeType m_shapeID;
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;
	virtual float getEnergy(glm::vec2 gravity) { return 0.0f; }
	virtual int getShapeID() { return (int)m_shapeID; }
	virtual void resetPosition() {};
	virtual void makeGizmo() {};
	
};

