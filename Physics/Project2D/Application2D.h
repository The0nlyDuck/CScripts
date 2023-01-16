#pragma once
#include "Application.h"
#include "PhysicsScene.h"
#include "Renderer2D.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//makes a rope of spheres
	void newRopeS(int num, glm::vec2 pos);
	//makes a rope of spinning boxes
	void newRopeB(int num, glm::vec2 pos);

	void ropeObjects(int num, RigidBody* b1, RigidBody* b2);

	glm::vec2 screenToWorld(glm::vec2 screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Texture*		m_carTexture;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	float m_timer;
};