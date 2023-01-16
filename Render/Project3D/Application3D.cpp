#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// set up camera
	m_camera.setPosition(vec3(10));
	m_camera.setTheta(45.f);
	m_camera.setPhi(210.f);
	
	m_light = Light();
	m_light.direction = glm::normalize(vec3(1, 1, 1));
	m_ambientLight = vec3(.1, .1, .1);

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), m_light, m_ambientLight);

	m_emitter = new ParticleEmitter();
	m_emitter->initialise(1000, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

	// red light on the left
	m_scene->getPointLights().push_back(Light(glm::vec3(3, 6, 3), glm::vec3(1, 0, 0), 50));
	// green light on the right
	m_scene->getPointLights().push_back(Light(glm::vec3(0, 6, 0), glm::vec3(.6, .6, .6), 50));

	//Texture Shader//
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_texturedShader.link() == false) {
		printf("Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}
	//---//

	//Partical Shader//
	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/particle.frag");
	if (m_particleShader.link() == false)
	{
		printf("Normal Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	//---//

	//Normal Map Shader//
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
	if (m_normalMapShader.link() == false) 
	{
		printf("Normal Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	//---//

	//making a quad//
	m_quadMesh.initialiseQuad();
	// make the quad 10 units wide 
	m_quadTransform = {
	 10,0,0,0,
	 0,10,0,0,
	 0,0,10,0,
	 0,0,0,1 };
	//---//

	//Loading Spear Mesh//
	if (m_spearMesh.load("./stanford/soulspear/soulspear.obj", true, true) == false) {
		printf("Spear Mesh Error!\n");
		return false;
	}

	for (int i = -2 ; i < 2; i++) 
	{
		for (int j = -2; j < 2; j++) 
		{
			glm::mat4 spearTransform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
			};
			spearTransform = glm::translate(spearTransform, glm::vec3(i * 3, 0, j * 3));
			Instance* in = new Instance(spearTransform, &m_spearMesh, &m_normalMapShader);

			m_scene->AddInstance(in);
		}
	}
	
	m_particleTransform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
	};


	//---//

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	int lightsNum = m_scene->getNumLights();
	std::vector<Light>& lights = m_scene->getPointLights();

	//Makes GUI to interact and change the lighting
	for (int i = 0; i < lightsNum; i++)
	{
		std::string text = "Light ";
		text += std::to_string(i + 1);

		ImGui::Begin(text.c_str());
		ImGui::Text("X:		  Y:		 Z:");
		ImGui::DragFloat3("Light Position", &lights.at(i).direction.x, 0.1f, -10.0f, 10.0f);
		ImGui::Text("R:		  G:		 B:");
		ImGui::DragFloat3("Light Colour", &lights.at(i).colour.x, 5.f, 0, 255.0f);
		ImGui::End();

	}

	ImGui::Begin("Adding/Removing Light"); 
	if (ImGui::Button("Add Light"))
	{
		m_scene->getPointLights().push_back(Light(vec3(0), vec3(5), 50));
	}
	if (ImGui::Button("Remove Back Light") && lightsNum > 1)
	{
		m_scene->getPointLights().pop_back();
	}
	ImGui::End();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	
	//Updates the camera and sets the matrixes after
	m_camera.update(deltaTime);
	m_projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = m_camera.getViewMatrix();

	//UI to see where to camera is and what it's rotation is
	ImGui::Begin("Camera Position/Rotation");
	ImGui::Text("Move camera with WASD, up and down with ZX.");
	ImGui::Text("Rotate the camera by holding the right mouse button and moving it.");
	ImGui::Text("X:");
	ImGui::Text(std::to_string(m_camera.getPosition().x).c_str());
	ImGui::Text("Y:");
	ImGui::Text(std::to_string(m_camera.getPosition().y).c_str());
	ImGui::Text("Z:");
	ImGui::Text(std::to_string(m_camera.getPosition().z).c_str());
	ImGui::Text("Theta:");
	ImGui::Text(std::to_string(m_camera.getTheta()).c_str());
	ImGui::Text("Phi:");
	ImGui::Text(std::to_string(m_camera.getPhi()).c_str());
	ImGui::End();

	//Updates emitter

	ImGui::Begin("Partical Settings");
	ImGui::Checkbox("Emitter On?", &m_emitterOn);
	ImGui::DragFloat3("Partical Position",&m_emitterPosition.x, 0.1f, -10.0f, 10.0f);
	ImGui::End();
	mat4 camTrans = Instance().makeTransform(m_camera.getPosition());
	if (m_emitterOn) 
	{
		m_emitter->update(deltaTime, camTrans, m_emitterPosition);
	}
	

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	//Closes the game
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour 
	clearScreen();
	
	drawMesh();
	if (m_emitterOn)
	{
		drawParticle();
	}	
	m_scene->draw();

	// draw 3D gizmos 
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix 
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::drawMesh()
{
	// Textured Shader
	m_texturedShader.bind();

	// bind transform 
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	// bind texture location 
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// bind texture to specified location 
	m_gridTexture.bind(0);

	// draw quad 
	m_quadMesh.draw();
}

void Application3D::drawParticle()
{
	// bind particle shader 
	m_particleShader.bind();

	// bind particle transform 
	auto pvm = m_projectionMatrix * m_viewMatrix * m_particleTransform;
	m_particleShader.bindUniform("ProjectionViewModel", pvm);

	m_emitter->draw();
}
