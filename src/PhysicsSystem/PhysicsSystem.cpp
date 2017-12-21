/*===================================================================================*//**
	PhysicsSystem
	
	The physics engine for the RACE game engine.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda
		
    @author Erick Fernandez de Arteaga, John Janzen
	@version 0.0.0
	@file
	
	@see PhysicsSystem
	@see PhysicsSystem.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "PhysicsSystem.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	Physics engine constructor.
///
PhysicsSystem::PhysicsSystem()
{
	subscribe(MESSAGE_TYPE::PhysicsCallMessageType);
	subscribe(MESSAGE_TYPE::PhysicsAccelerateCallType);
}

///
///	Physics engine destructor.
///
PhysicsSystem::~PhysicsSystem()
{
	SDL_Log("%s", "Running Physics::Destructor");
}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///	Starts the physics engine loop on its own thread and returns a pointer to the thread.
///
std::thread* PhysicsSystem::start()
{
	if (!_running)
	{
		_running = true;
	}

	return new std::thread(&PhysicsSystem::loop, this);
}

///
///	The physics engine loop.
///
void PhysicsSystem::loop()
{
	while (_running) {
		_urgentMessageQueueMutex_p->lock();
		_messageQueueMutex_p->lock();
		if (_messageQueue.empty() && _urgentMessageQueue.empty() && _running) 
		{
			_urgentMessageQueueMutex_p->unlock();
			_messageQueueMutex_p->unlock();
			std::this_thread::yield();
		}
		else if (!_running) 
		{
			_urgentMessageQueueMutex_p->unlock();
			_messageQueueMutex_p->unlock();
		}
		else {
			if (!_urgentMessageQueue.empty())
			{
				_messageQueueMutex_p->unlock();
				// process an urgent message


				_urgentMessageQueueMutex_p->unlock();
			}
			else
			{
				_urgentMessageQueueMutex_p->unlock();
				if (!_messageQueue.empty())
				{
					// process a normal messages
					checkMessage(_messageQueue.front());
					_messageQueue.pop();
					
				}
				_messageQueueMutex_p->unlock();
			}
		}
	}
	this->stop();
	SDL_Log("Physics::Out of loop");
};

void PhysicsSystem::checkMessage(std::shared_ptr<Message> myMessage) 
{

	switch (myMessage->getType()) {
	case MESSAGE_TYPE::PhysicsCallMessageType:
	{
		PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
		_deltaTime = content->deltaTime;
		for (std::map<std::string, GameObject*>::iterator it = content->worldObjects.begin(); it != content->worldObjects.end(); ++it) {
			GameObject* go = it->second;
			generalPhysicsCall(go);
			collisionDetection(content->worldObjects, go);
		}
		std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsReturnCall));
		MessagingSystem::instance().postMessage(myMessage);
		break;
	}
	case MESSAGE_TYPE::PhysicsAccelerateCallType: 
	{
		/* Get player input data. */
		PhysicsAccelerateContent* content = static_cast<PhysicsAccelerateContent*>(myMessage->getContent());
		GameObject* go = content->object;
		GLfloat forward = content->amountFast;
		GLfloat reverse = content->amountSlow;
		RigidBodyComponent *rbc = go->getComponent<RigidBodyComponent*>();
		Vector3 F_Long;
		GLfloat turningDegree = content->turningDegree;
		bool isDrifting = content->_isDrifting;
		bool wasDrifting = content->_wasDrifting;

		/* If drifting, DRIIIIIFFFFFFTTTT!. */
		if (isDrifting || wasDrifting)
		{
			/* If we are entering a drift, reset the drift timer. */
			if (!wasDrifting)
			{
				_driftTimer = 0;
			}

			/* If we just started a drift or are in the middle of one, turn faster. */
			if (isDrifting)
			{
				/* During a drift, your brake hard and slow down, and
					turn faster for sharper cornering. This works out to be slower than 
					normal acceleration, but you get a boost of speed coming out of the drift.
				*/
				//F_Long = go->_transform._forward * (forward * 300);
				F_Long += -go->_transform._forward * (reverse * 500);
				rbc->setForce(F_Long);

				rbc->setTurningDegree(turningDegree * 1.1f); // Turning input from user
				_driftTimer += _deltaTime;
			}
			/* If we're exiting a drift, apply the speed boost. */
			else if (_driftTimer > 1.5f)
			{
				/* The drift boost multiplier is between 1.5 - 2.0 for drift times over 2.0 seconds. */
				float driftMultiplier = 1.0f + (1.0f * fmin(_driftTimer, 4.0f) / 4.0f);

				/* Calculate the speed boost, but make sure the car doesn't exceed its max velocity. */
				Vector3 candidateVelocity = go->_transform._forward * rbc->getVelocity().magnitude() * driftMultiplier;
				if (candidateVelocity.magnitude() > rbc->getMaxVelocity() * 1.1f)
				{
					candidateVelocity = candidateVelocity.normalize() * rbc->getMaxVelocity() * 1.1f;
				}

				/* Update the velocity. */
				rbc->setVelocity(candidateVelocity);
			}
		}
		/* If not drifting, steer normally. */
		else
		{
			if (forward != 0)
			{
				F_Long = go->_transform._forward * (forward * 2000);
			}
			if (reverse != 0)
			{
				F_Long = -go->_transform._forward * (reverse * 4000);
			}
			rbc->setForce(F_Long);
			rbc->setTurningDegree(turningDegree); // Turning input from user
		}

		break;
	}
	default:
		break;
	}
};

void PhysicsSystem::generalPhysicsCall(GameObject* go) 
{
	if (go->hasComponent<RigidBodyComponent*>()) 
	{
		RigidBodyComponent* rbc = go->getComponent<RigidBodyComponent*>();
		if (go->_name.compare("sphere") == 0) {
			rbc->setAngularAccel(Vector3(0, 0, PI) * 0.5 * _deltaTime);
		}
		
		adjustForces(go, rbc);
		applyAcceleration(go, rbc);
		go->translate(rbc->getVelocity() * _deltaTime);
		go->rotate(rbc->_angularVel * 0.5 * _deltaTime);
		if (go->_name.compare("player") == 0 && rbc->getVelocity().magnitude() >= 0) {
			turnGameObject(go);
		}
	}
};

void PhysicsSystem::applyAcceleration(GameObject *go, RigidBodyComponent *rc) 
{
	if (rc->getVelocity().magnitude() < rc->getMaxVelocity()) 
	{
		linearAccelerate(go,rc);
		//SDL_Log("SPEED: %f", rc->getVelocity().magnitude());
	}
	angularAccelerate(rc);
};

void PhysicsSystem::adjustForces(GameObject *go, RigidBodyComponent *rc) 
{
	Vector3 dragVector = -rc->getVelocity().normalize();
	Vector3 newForce = rc->getForce() + (dragVector * (rc->getVelocity().magnitude() *
		rc->getVelocity().magnitude()) * RHO * LINEARDRAGCOEF * ((rc->_length / 2.0f) * (rc->_length / 2.0f)));
	//newForce = QVRotate(go->_transform._orientation, newForce);

	rc->setAccelerationVector(newForce / rc->getMass());

	Vector3 angularDragVector = -rc->_angularVel.normalize();
	rc->_angularMoment += (angularDragVector * (rc->_angularVel.magnitude() *
		rc->_angularVel.magnitude()) * RHO * ANGULARDRAGCOEF * ((rc->_length / 2.0f) * (rc->_length / 2.0f)));

	glm::vec3 inertiaAngVel = rc->_mInertia * glm::vec3(rc->_angularVel.x, rc->_angularVel.y, rc->_angularVel.z);
	Vector3 angMoments = rc->_angularMoment - rc->_angularVel.crossProduct(Vector3(inertiaAngVel.x, inertiaAngVel.y, inertiaAngVel.z));
	glm::vec3 something = rc->_mInertiaInverse * glm::vec3(angMoments.x, angMoments.y, angMoments.z);
	rc->setAngularAccel(rc->getAngularAccel() + Vector3(something.x, something.y, something.z));
};

/**
 *	Stops the physics engine.
 */
void PhysicsSystem::stop() {};

void PhysicsSystem::flagLoop() 
{
	_running = false;
};

/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsSystem::linearAccelerate(GameObject* go, RigidBodyComponent* rbc)
{
	rbc->setVelocity(go->_transform._forward * rbc->getVelocity().magnitude() + rbc->getAccelerationVector() * _deltaTime);
};


void PhysicsSystem::angularAccelerate(RigidBodyComponent* rbc) 
{
	//rbc->setAngularAccel();
	rbc->_angularVel += rbc->getAngularAccel() * _deltaTime;
};

/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsSystem::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	//go->getComponent<RigidBodyComponent*>()->getVelocity() += Vector3(x, y, z) * _deltaTime;
	//go->getComponent<RigidBodyComponent*>()->setSpeed(go->getComponent<RigidBodyComponent*>()->getAccNumber() * _deltaTime);
};

Vector3 PhysicsSystem::getAngleFromTurn(GameObject *go, GLfloat tireDegree)
{
	GLfloat objectVelocity = go->getComponent<RigidBodyComponent*>()->getVelocity().magnitude();

	GameObject *wheelRL = go->getChild("wheelRL");
	GameObject *wheelFL = go->getChild("wheelFL");
	GameObject *wheelFR = go->getChild("wheelFR");

	GLfloat L = (wheelFL->_transform._position - wheelRL->_transform._position).magnitude();

	if (tireDegree == 0)
	{
		if (wheelFL->_transform._rotation.y < -0.0001)
			wheelFL->_transform._rotation.y += (PI / 4.0f) * _deltaTime;
		else if (wheelFL->_transform._rotation.y > 0.0001)
			wheelFL->_transform._rotation.y -= (PI / 4.0f) * _deltaTime;
		else
			wheelFL->_transform._rotation.y = 0.0f;

		if (wheelFR->_transform._rotation.y < -0.0001)
			wheelFR->_transform._rotation.y += (PI / 4.0f) * _deltaTime;
		else if (wheelFR->_transform._rotation.y > 0.0001)
			wheelFR->_transform._rotation.y -= (PI / 4.0f) * _deltaTime;
		else
			wheelFR->_transform._rotation.y = 0.0f;

		return Vector3(0, 0, 0);
	}
	GLfloat sinTheta = sin(tireDegree);
	GLfloat denominator = (L / (sinTheta));
	GLfloat omega = objectVelocity / denominator;
	wheelFL->_transform._rotation.y = tireDegree;
	wheelFR->_transform._rotation.y = tireDegree;
	
	return go->_transform._up * omega;
};

void PhysicsSystem::turnGameObject(GameObject *go)
{
	Vector3 angularVelocity = getAngleFromTurn(go, go->getComponent<RigidBodyComponent*>()->getTurningDegree());
	if (go->getComponent<RigidBodyComponent*>()->getVelocity().magnitude() > 0)
		go->rotate(angularVelocity, 0.5 * _deltaTime); // angularVelocity * deltaTime = current angle
}

void PhysicsSystem::collisionDetection(std::map<std::string, GameObject*> worldObj, GameObject * go)
{
	if (go->hasComponent<BoxColliderComponent*>()) 
	{
		for (std::map<std::string, GameObject*>::iterator it = worldObj.begin(); it != worldObj.end(); ++it) 
		{
			if (it->second->hasComponent<BoxColliderComponent*>() && it->second != go)
			{
				if (checkForCollision(go, it->second)) 
				{
					if (it->second->hasComponent<RigidBodyComponent*>()) 
					{
						it->second->getComponent<RigidBodyComponent*>()->setForce(Vector3(1.0f, 1.0f, 0.0f) * _deltaTime);
					}
					if (go->hasComponent<RigidBodyComponent*>())
					{
						//go->getComponent<RigidBodyComponent*>()->setVelocity();
						SDL_Log("Collision on Car");
					}
				}
			}
		}
	}
};

bool PhysicsSystem::checkForCollision(GameObject *coll1, GameObject *coll2) 
{
	BoxColliderComponent *boxColl = coll1->getComponent<BoxColliderComponent*>();
	BoxColliderComponent *boxColl2 = coll2->getComponent<BoxColliderComponent*>();
	Vector3 obj1Pos = coll1->_transform._position;
	Vector3 obj2Pos = coll2->_transform._position;

	return (obj1Pos.x + boxColl->getMaxX() > obj2Pos.x + boxColl2->getMinX() &&
		obj1Pos.x + boxColl->getMinX() < obj2Pos.x + boxColl2->getMaxX() &&
		obj1Pos.y + boxColl->getMaxY() > obj2Pos.y + boxColl2->getMinY() &&
		obj1Pos.y + boxColl->getMinY() < obj2Pos.y + boxColl2->getMaxY() &&
		obj1Pos.z + boxColl->getMaxZ() > obj2Pos.z + boxColl2->getMinZ() &&
		obj1Pos.z + boxColl->getMinZ() < obj2Pos.z + boxColl2->getMaxZ());
};