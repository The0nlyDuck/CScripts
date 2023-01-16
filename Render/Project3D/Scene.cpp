#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light light, glm::vec3 ambientLight)
{
	m_camera = camera;
	m_windowSize = windowSize;
	m_sunLight = light;
	m_ambientLight = ambientLight;
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		delete* it;
}

void Scene::AddInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->draw(this);
	}
}

