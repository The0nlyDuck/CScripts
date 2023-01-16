#include "PhysicsScene.h"
#include "glm/glm.hpp"

PhysicsScene::PhysicsScene():m_timeStep(0.01f), m_gravity(glm::vec2(0.0f, 0.0f))
{
	
	
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor:m_actors) delete pActor;
	
}

//Adds Physic object into actors vector
void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

//Goes through all the actors and removes it from the vector
void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++) 
	{
		//found
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
		}
	}
	//Not found
}

// function pointer array for doing our collisions 
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
 PhysicsScene::plane2Plane,	 PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
 PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere,PhysicsScene::sphere2Box,
 PhysicsScene::box2Plane,    PhysicsScene::box2Sphere,    PhysicsScene::box2Box,
};

void PhysicsScene::update(float dt)
{
	// update physics at a fixed time step 
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}

	//check for collisions (ideally you'd want to have some sort of scene management in place) 
	int actorCount = m_actors.size();


	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++) 
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = object1->getShapeID();
			int shapeID2 = object2->getShapeID();

			// this check will ensure we don't include any joints in the collision checks
			if (shapeID1 < 0 || shapeID2 < 0) continue;

			// using function pointers
			int functionIdx = (shapeID1 * (int)ShapeType::SHAPE_COUNT) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur? 
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

void PhysicsScene::draw()
{
	for (auto pActor : m_actors) 
	{
		pActor->draw();
	}
}

float PhysicsScene::getTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++) 
	{
		PhysicsObject* obj = *it;
		total += obj->getEnergy(getGravity());
	}
	return 0.0f;
}

//---//
bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr) 
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		//get a representative point on the plane
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();

		//chech all four corners to see if we've hit the plane
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth()) 
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight()) 
			{
				// get the position of the corner in world space 
				glm::vec2 p = box->getPosition() + x * box->getXLocal() + y * box->getYLocal();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				// this is the total velocity of the point in world space 
				glm::vec2 displacement = x * box->getXLocal() + y * box->getYLocal();
				glm::vec2 pointVelocity = box->getVelocity() + box->getAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
				// and this is the component of the point velocity into the plane 
				float velocityIntoPlane = glm::dot(pointVelocity, plane->getNormal());

				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}
		// we've had a hit - typically only two corners can contact 
		if (numContacts > 0)
		{
			plane->resolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	return false;
}

//---//

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and plane
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 spherePos = sphere->getPosition();
		glm::vec2 collisionNormal = plane->getNormal();
		float planeDistance = plane->getDistance();

		float sphereToPlane = glm::dot(spherePos, collisionNormal) - planeDistance;

		float intersection = sphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->getVelocity(), collisionNormal);
		
		if (intersection > 0.1f && velocityOutOfPlane < 0.1f)
		{
			plane->resolveCollision(sphere,sphere->getPosition() + (-sphere->getRadius() * collisionNormal));
		}
	}

	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{		
		glm::vec2 dist = sphere1->getPosition() - sphere2->getPosition();
		glm::vec2 normal = glm::normalize(dist);
		if (glm::length(dist) < sphere1->getRadius() + sphere2->getRadius()) {
			// collision 
			// for now, just stop the spheres 
			glm::vec2 contact = sphere1->getPosition() - (normal * sphere1->getRadius());
			sphere1->resolveCollision(sphere2, contact);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return box2Sphere(obj2,obj1);
}

//---//

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return plane2Box(obj2, obj1);
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	if (box != nullptr && sphere != nullptr)
	{
		// transform the circle into the box's coordinate space 
		glm::vec2 circlePosWorld = sphere->getPosition() - box->getPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->getXLocal()), glm::dot(circlePosWorld, box->getYLocal()));

		// find the closest point to the circle centre on the box by clamping the coordinates in box - space to the box's extents
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->getExtents();
		if(closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
		if(closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if(closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
		if(closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;

		//convert back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->getPosition() + closestPointOnBoxBox.x * box->getXLocal() + closestPointOnBoxBox.y * box->getYLocal();
		glm::vec2 circleToBox = sphere->getPosition() - closestPointOnBoxWorld;

		if (glm::length(circleToBox) < sphere->getRadius())
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->resolveCollision(sphere, contact, &direction);
		}
	}
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->getPosition() - box1->getPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;

		box1->checkBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm)) 
		{
			norm = -norm;
		}
		if (pen > 0) {
			box1->resolveCollision(box2, contact / float(numContacts), &norm);
		}
		return true;
	}
	return false;
}
//---//

