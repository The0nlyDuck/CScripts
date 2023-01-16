#include "Application2D.h"
#include <glm/ext.hpp>
#include "Texture.h"
#include "Font.h"
#include "Input.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

float randdec() 
{
	return (float)rand() / RAND_MAX;
}

bool Application2D::startup() {

	// increase the 2d line to maximize the number of objects to draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0,-9));
	m_physicsScene->setTimeStep(0.01f);

	//Instantiation a ball then adds it to the Physics scene
	Sphere* ball1 = new Sphere(glm::vec2(10, 0), glm::vec2(-5, 0), 50.0f, 1, 5, glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(ball1);
	
	Sphere* ball2 = new Sphere(glm::vec2(-10, 0), glm::vec2(10, 0), 5.0f, 1, 0, glm::vec4(1, 1, 0, 1));
	m_physicsScene->addActor(ball2);

	//Instantiation a box then adds it to the Physics scene
	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(0, 0), 5.0f, 5, 2, 0, 10.f, glm::vec4(1, 1, 0, 1));
	m_physicsScene->addActor(box1);
	
	Box* box2 = new Box(glm::vec2(-20, 20), glm::vec2(0, -10), 5.0f, 5, 2, 0, 0, glm::vec4(0, 1, 0.5, 1));
	m_physicsScene->addActor(box2);

	//Instantiation a spring by connecting two rigidbodies before adding it to the scene
	Spring* spring = new Spring(ball1, box1, 5, 10);
	m_physicsScene->addActor(spring);

	//Creates two ropes, one made of spheres, the other rotating cubes
	newRopeS(10, glm::vec2(-15, 30));
	newRopeB(10, glm::vec2(15, 30));

	//Creates a line of objects attached by springs
	Box* leftBox = new Box(glm::vec2(-50, 40), glm::vec2(0, 0), 5.0f, 5, 5, 0, 0, glm::vec4(1, 1, 0.5, 1));
	m_physicsScene->addActor(leftBox);

	Box* rightBox = new Box(glm::vec2(50, 40), glm::vec2(0, -10), 5.0f, 5, 5, 0, 0, glm::vec4(1, 1, 0.5, 1));
	m_physicsScene->addActor(rightBox);

	ropeObjects(4,leftBox,rightBox);

	//Instantiates planes that act like walls and are added to the Physics scene
	Plane* rightWall = new Plane(glm::normalize(glm::vec2(1, 0)), -75, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(rightWall);
	
	Plane* leftWall = new Plane(glm::normalize(glm::vec2(-1, 0)), -75, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(leftWall);

	Plane* topWall = new Plane(glm::normalize(glm::vec2(0, 1)), -50, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(topWall);
	
	Plane* bottomWall = new Plane(glm::normalize(glm::vec2(0, -1)), -50, glm::vec4(0, 1, 1, 1));
	m_physicsScene->addActor(bottomWall);

	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//updates all the physics object in the scene
	m_physicsScene->update(deltaTime);

	// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	//Mouse pos
	glm::vec2 pos = { input->getMouseX(),input->getMouseY() };
	pos = screenToWorld(pos);
	aie::Gizmos::add2DCircle(pos, 2, 10, { 1,0,1,0 });

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);

	//Draws a circle at the mouse position when clicked
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		Sphere* ball = new Sphere(pos, glm::vec2(0, 0), 5.0f, 1, 0, glm::vec4(randdec(), randdec(), randdec(), 1.f));
		m_physicsScene->addActor(ball);
	}
	//Draws a box at the mouse position when clicked
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		Box* box = new Box(pos, glm::vec2(0, 0), 5.0f, 5, 5, (float)rand() / 360, 0, glm::vec4(randdec(), randdec(), randdec(), 1.f));
		m_physicsScene->addActor(box);
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	m_physicsScene->draw();

	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));


	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Left Click to place Circle!", 0, 720 - 64);
	m_2dRenderer->drawText(m_font, "Right Click to place Box!", 0, 720 - 96);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 128);

	// done drawing sprites
	m_2dRenderer->end();
}

//Creates a rope that is spheres connected by ropes
void Application2D::newRopeS(int num, glm::vec2 pos)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < num; i++) 
	{
		// spawn a sphere to the right and below the previous one, so that the whole rope acts under gravityand swings
		Sphere* sphere = new Sphere(glm::vec2(pos.x - i * 3, pos.y - i * 5), glm::vec2(0), 10.0f, 1, 0, glm::vec4(1, 0, 0, 1));
		if(i == 0) sphere->setKinematic(true);
		m_physicsScene->addActor(sphere);
		Spring* spring = new Spring(sphere, prev, 7, 500);
		if (prev) m_physicsScene->addActor(spring);
		prev = sphere;
	}
}

//Creates a rope that is boxes connected by ropes
void Application2D::newRopeB(int num, glm::vec2 pos)
{
	Box* prev = nullptr;
	for (int i = 0; i < num; i++)
	{
		// spawn a box to the right and below the previous one, so that the whole rope acts under gravityand swings
		Box* box = new Box(glm::vec2(pos.x - i * 3, pos.y - i * 5), glm::vec2(0), 10.0f, 1, 5, 5, 45, glm::vec4(1, 0, 0, 1));
		if (i == 0) box->setKinematic(true);
		m_physicsScene->addActor(box);
		Spring* spring = new Spring(box, prev, 7, 500);
		if (prev) m_physicsScene->addActor(spring);
		prev = box;
	}
}

void Application2D::ropeObjects(int num, RigidBody* b1, RigidBody* b2)
{
	RigidBody* prev = b1;
	Box* box = new Box(glm::vec2(prev->getPosition().x + 5, prev->getPosition().y), glm::vec2(0), 10, 2, 2, 0, 15, glm::vec4(1,1,0,1));
	m_physicsScene->addActor(box);
	Spring* spring = new Spring(box, prev, 3, 10);
	if (prev) m_physicsScene->addActor(spring);
	prev = box;
	for (int i = 1; i < num; i++)
	{
		box = new Box(glm::vec2(prev->getPosition().x + (5 * num), prev->getPosition().y), glm::vec2(0), 10, 2, 2, 0, 15, glm::vec4(1, 1, 0, 1));
		m_physicsScene->addActor(box);
		spring = new Spring(box, prev, 3, 10);
		if (prev) m_physicsScene->addActor(spring);
		prev = box;
	}
	spring = new Spring(b2, prev, 7, 500);
	if (prev) m_physicsScene->addActor(spring);
}

glm::vec2 Application2D::screenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	worldPos.x *= 2.0f * 100 / getWindowWidth();
	worldPos.y *= 2.0f * 100 / ((16 / 9.f) * getWindowHeight());

	return worldPos;
}
