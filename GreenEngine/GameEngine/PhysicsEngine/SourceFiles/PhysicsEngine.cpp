/*===================================================================================*//**
	PhysicsEngine
	
	The physics engine for the RACE game engine.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda
		
    @author Erick Fernandez de Arteaga, John Janzen
	@version 0.0.0
	@file
	
	@see PhysicsEngine
	@see PhysicsEngine.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "PhysicsEngine.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	Physics engine constructor.
///
PhysicsEngine::PhysicsEngine()
{
	subscribe(MESSAGE_TYPE::PhysicsCallMessageType);
	subscribe(MESSAGE_TYPE::InputMessageType);
	subscribe(MESSAGE_TYPE::PhysicsInitializeCallType);
}

///
///	Physics engine destructor.
///
PhysicsEngine::~PhysicsEngine()
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
std::thread* PhysicsEngine::start()
{
	if (!_running)
	{
		_running = true;
	}

	return new std::thread(&PhysicsEngine::loop, this);
}

///
///	The physics engine loop.
///
void PhysicsEngine::loop()
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
}

void PhysicsEngine::checkMessage(std::shared_ptr<Message> myMessage) {
	

	switch (myMessage->getType()) {
	case MESSAGE_TYPE::PhysicsCallMessageType:
	{
		PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
		
		for (std::map<std::string, GameObject*>::iterator it = content->worldObjects.begin(); it != content->worldObjects.end(); ++it) {
			GameObject* go = it->second;
			generalPhysicsCall(go);
			if (it->first == "Sphere") {
				rotate(it->second, Vector3(0, MATH_PI, 0) * content->deltaTime);
			}
		}
		_deltaTime = content->deltaTime;
		break;
	}
	case MESSAGE_TYPE::InputMessageType:
	{
		InputMessageContent* content = static_cast<InputMessageContent*>(myMessage->getContent());
		getControllerInput(content);
	}
		break;
	case MESSAGE_TYPE::PhysicsInitializeCallType:
	{
		PhysicsInitializeContent* content = static_cast<PhysicsInitializeContent*>(myMessage->getContent());
		_camera_p = content->camera;
		_player_p = content->player;
	}
	break;
	default:
		break;
	}
}

void PhysicsEngine::getControllerInput(InputMessageContent *content) {
	switch (content->type) {
	case INPUT_TYPES::LOOK_AXIS: 
	{
		//SDL_Log("A Button Pressed");
		rotate(_camera_p, Vector3(content->lookY, content->lookX, 0) * 2 * _deltaTime);
		break;
	}
	case INPUT_TYPES::MOVE_AXIS: 
	{
		glm::mat4x4 matrix = glm::eulerAngleXYZ(_camera_p->_transform.getRotation().x, _camera_p->_transform.getRotation().y, _camera_p->_transform.getRotation().z);
		translate(_camera_p, Vector3(content->lookX, 0, content->lookY).matrixMulti(matrix) * 2 * _deltaTime);
	}
		break;
	case INPUT_TYPES::TRIGGERS: 
	{
		if (_player_p->getComponent<AccelerationComponent*>()->_acceleration.magnitude() < _player_p->getComponent<AccelerationComponent*>()->_maxAcceleration) {
			_player_p->getComponent<AccelerationComponent*>()->_acceleration += Vector3(_player_p->_transform._forward) * content->lookY * _deltaTime;
		}
	}
		break;
	default:
		break;
	}
}

void PhysicsEngine::generalPhysicsCall(GameObject* go) {
	if (go->hasComponent<AccelerationComponent*>() && go->hasComponent<VelocityComponent*>()) {
		applyAcceleration(go);
		translate(go, go->getComponent<VelocityComponent*>()->_velocity);
	}
}

void PhysicsEngine::applyAcceleration(GameObject* go) {
	VelocityComponent* vc = go->getComponent<VelocityComponent*>();
	if (vc->_velocity.magnitude() < vc->_maxVelocity) {
		accelerate(go, Vector3(go->_transform._forward) * go->getComponent<AccelerationComponent*>()->_acceleration);
	}
		
	//accelerate(go, go->);
}

/**
 *	Stops the physics engine.
 */
void PhysicsEngine::stop()
{
	if (_camera_p != nullptr)
		delete(_camera_p);
	if (_player_p != nullptr)
		delete(_player_p);
	//SDL_Log("Physics::Stop");
}

void PhysicsEngine::flagLoop() {
	_running = false;
}
/**
 *  <summary>
 *  Move the game object in a direciton. The translation should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::translate(GameObject *go, Vector3 translation)
{
	go->_transform._position += translation;
};

void PhysicsEngine::translateForward(GameObject *go, Vector3 translation) 
{
	go->_transform._position += (translation * go->_transform.getForward());
};
/**
 *  <summary>
 *  Move the game object in a direciton. Each axis should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::translate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->_transform._position += Vector3(x, y, z);
};
/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<VelocityComponent*>()->_velocity += amount;
};
/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<VelocityComponent*>()->_velocity += Vector3(x, y, z);
};
void PhysicsEngine::decelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= amount;
};
void PhysicsEngine::decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= Vector3(x, y, z);
};
/**
 * <summary>
 * Rotate the object by a set amount. This rotation is in radians only, 
 * and is only ever increasing the rotation.
 * Please specify positive/negative when calling. e.g., rotate(&go, -1.2);
 * </summary>
 */
void PhysicsEngine::rotate(GameObject *go, Vector3 amount)
{
	glm::mat4x4 matrix = glm::eulerAngleXYZ(amount.x, amount.y, amount.z);
	glm::vec4 temp = glm::vec4(go->_transform._forward.x, go->_transform._forward.y, go->_transform._forward.z, 1) * matrix;
	glm::vec4 temp2 = glm::vec4(go->_transform._right.x, go->_transform._right.y, go->_transform._right.z, 1) * matrix;
	glm::vec4 temp3 = glm::vec4(go->_transform._up.x, go->_transform._up.y, go->_transform._up.z, 1) * matrix;
	go->_transform._forward = Vector3(temp.x, temp.y, temp.z);
	go->_transform._right = Vector3(temp2.x, temp2.y, temp2.z);
	go->_transform._up = Vector3(temp3.x, temp3.y, temp3.z);
	go->_transform._rotation += amount;
};
void PhysicsEngine::rotateX(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.x += angle;
};
void PhysicsEngine::rotateY(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.y += angle;
};
void PhysicsEngine::rotateZ(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.z += angle;
};