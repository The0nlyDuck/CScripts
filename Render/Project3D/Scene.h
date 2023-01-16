#pragma once
#include <list>
#include "Camera.h"
#include <glm/mat4x4.hpp>

#define MAX_LIGHTS	256

struct Light {
	glm::vec3 direction;
	glm::vec3 colour;
	
	Light()
	{
		direction = glm::vec3(1);
		colour = glm::vec3(1);
	}
	
	Light(glm::vec3 pos, glm::vec3 col, float intensity)
	{
		direction = pos;
		colour = col * intensity;
	}
};

class Instance;

class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize, Light light, glm::vec3 ambientLight);
	~Scene();

	void AddInstance(Instance* instance);
	std::list<Instance*> getInstanceList() { return m_instances; }
	void draw();
	
	Camera* getCamera() { return m_camera; }
	glm::vec2 getWindowSize() { return m_windowSize; }

	int getNumLights() { return m_pointLights.size(); }

	glm::vec3* getPointlightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointlightColours() { return &m_pointLightColours[0]; }

	Light getSunLight() { return m_sunLight; }
	glm::vec3 getAmbientLight() { return m_ambientLight; }

	std::vector<Light>& getPointLights() { return m_pointLights; }

	

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	std::list<Instance*> m_instances;

	Light m_sunLight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
};

