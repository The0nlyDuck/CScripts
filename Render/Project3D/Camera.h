#pragma once
#include <glm/ext.hpp>
#include "Input.h"
#include "glm/trigonometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class Camera
{
public:
	void update(float deltaTime);

	void setPosition(glm::vec3 position) { m_position = position; }
	glm::vec3 getPosition() { return m_position; }

	void setTheta(float theta) { m_theta = theta; }
	float getTheta() { return m_theta; }
	void setPhi(float phi) { m_phi = phi; }
	float getPhi() { return m_phi; }

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float w, float h);

private:
	float m_lastMouseX = 0;
	float m_lastMouseY = 0;
	float m_theta = 0;
	float m_phi = 0;
	glm::vec3 m_position;
};

