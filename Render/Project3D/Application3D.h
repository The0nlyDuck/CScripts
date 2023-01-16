#pragma once
#include <iostream>
#include "Application.h"
#include "Mesh.h"
#include "imgui.h"
#include "Scene.h"
#include "Instance.h"
#include "ParticleEmitter.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void drawMesh();
	void drawParticle();

protected:

	float m_c = 0;
	bool m_reverse = false;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	aie::ShaderProgram m_particleShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normalMapShader;

	Scene* m_scene;

	ParticleEmitter* m_emitter;
	vec3 m_emitterPosition = vec3(0);
	bool m_emitterOn = true;

	Mesh     m_quadMesh;
	glm::mat4   m_quadTransform;

	glm::mat4   m_particleTransform;

	aie::OBJMesh  m_spearMesh;

	Light	m_light;
	glm::vec3   m_ambientLight;

	Camera m_camera;
};