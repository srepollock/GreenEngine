#include "InputEngine.h"

InputEngine::InputEngine() {
	subscribe(MESSAGE_TYPE::InputInitializeCallType);
	subscribe(MESSAGE_TYPE::InputButtonDownCallType);
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	int numJoysticks = SDL_NumJoysticks();
	if (numJoysticks != 0)
	{
		for (int x = 0; x < numJoysticks; x++)
		{
			if (SDL_IsGameController(x))
			{
				gameController = SDL_GameControllerOpen(x);
				break;
			}
		}
	}
	else
	{
		gameController = NULL;
	}
}

void InputEngine::setUpInput() 
{
	
	while (!_messageQueue.empty()) 
	{
		std::shared_ptr<Message> myMessage = _messageQueue.front();

		switch (myMessage->getType()) 
		{
			case MESSAGE_TYPE::InputInitializeCallType:
			{
				InputInitializeContent* content = static_cast<InputInitializeContent*>(myMessage->getContent());
				_camera_p = content->camera;
				_player_p = content->player;
				_playerToCamera = Vector3(_camera_p->_transform._position - _player_p->_transform._position);
				_camera_p->_transform._position = _playerToCamera + _player_p->_transform._position;
				GLfloat angleY = (GLfloat)atan2(_playerToCamera.z, _playerToCamera.x);
				_camera_p->_transform._orientation.MakeQFromEulerAngles(0, angleY - PI / 2.0f, 0);
				_messageQueue.pop();
				break;
			}
		}
	}
}

InputEngine::~InputEngine() 
{
	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}
}

void InputEngine::buttonEventHandler(SDL_Event ev)
{
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_Y: 
		{
			InputMessageContent *inputContent = new InputMessageContent();
			inputContent->type = INPUT_TYPES::Y_BUTTON;
			std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType, false));
			inputMessage->setContent(inputContent);
			MessagingSystem::instance().postMessage(inputMessage);
			break;
		}
		case SDL_CONTROLLER_BUTTON_X :
		{
			/* Flip the camera rotation to toggle looking forwards/back. */
			_lookBack = !_lookBack;

			break;
		}
		case SDL_CONTROLLER_BUTTON_BACK: 
		{
			cameraIndependant = !cameraIndependant;
			if (!cameraIndependant) {
				GLfloat angleY = atan2(_playerToCamera.z, _playerToCamera.x);
				_camera_p->_transform._rotation.y = angleY - PI / 2.0f;
			}
			else {
				//_camera_p->_transform._forward = _playerToCamera.normalize();
				//_camera_p->_transform._forward.y = 0;
			}
			
			break;
		}
		default:
			break;
	}
}

void InputEngine::axisEventHandler(GLfloat X, GLfloat Y, INPUT_TYPES type)
{
	switch (type) {
	case INPUT_TYPES::LOOK_AXIS:
	{
		//SDL_Log("A Button Pressed");
		if (this->cameraIndependant)
		{
			//rotate(_camera_p, Vector3(content->lookY, content->lookX, 0) * 2 * _deltaTime);
			//_camera_p->_transform.rotate(Vector3(-Y, X, 0) * 2 * _deltaTime);
			//_camera_p->_transform.rotateQuat(Vector3(-Y, -X, 0), _deltaTime);
			if (X != 0 || Y != 0) {
				_camera_p->_transform._orientation = _camera_p->_transform._orientation * MakeQFromEulerAngles(Vector3(Y, X, 0) * _deltaTime);
				_camera_p->_transform._orientation.Normalize();
			}
		}
		else
		{
			if (X != 0 || Y != 0)
			{
				/* Commenting this code out so that the player can't turn the camera.
					If you need to uncomment this, leave the double-commented lines commented.
				*/

				//_playerToCamera = _camera_p->_transform.rotateAround(_playerToCamera, _player_p->_transform._position, Vector3(0.0f, -X * _deltaTime, 0.0f));
				//GLfloat angleY = atan2(_playerToCamera.z, _playerToCamera.x);
				////GLfloat angleX = atan2(sqrtf(powf(_playerToCamera.z, 2) + powf(_playerToCamera.x, 2)), _playerToCamera.y);
				////angleY = PI - (angleY);
				//_camera_p->_transform._orientation.MakeQFromEulerAngles(0.0f, angleY - PI / 2.0f, 0.0f);
				if (!_lookBack) 
				{
					_lookAngle += -X * _deltaTime;
					if (_lookAngle >= 2 * PI)
						_lookAngle -= 2 * PI;
				}
			}
			else 
			{
				if (_lookAngle > 0.01f)
					_lookAngle -= (PI / 4.0f) * _deltaTime;
				else if (_lookAngle < -0.01f) 
					_lookAngle += (PI / 4.0f) * _deltaTime;
				else 
					_lookAngle = 0.0f;
			}
		}
		break;
	}
	case INPUT_TYPES::MOVE_AXIS:
	{
		if (this->cameraIndependant)
		{
			glm::mat4x4 matrix = glm::eulerAngleXYZ(_camera_p->_transform.getRotation().x, _camera_p->_transform.getRotation().y, _camera_p->_transform.getRotation().z);
			_camera_p->_transform.translate(Vector3(X, 0, Y).matrixMulti(matrix) * 2 * _deltaTime);
		}
		else
		{
			/* Turn the player car. */
			//_player_p->_transform.rotateY(-X * _deltaTime);
			if (X != 0) {
				_turningDegree += -X * (PI / 4.0f) * _deltaTime;
				if (_turningDegree >= (PI / 4.0f))
					_turningDegree = (PI / 4.0f);
				else if (_turningDegree <= -(PI / 4.0f))
					_turningDegree = -(PI / 4.0f);
			}
			else {
				
				if (_turningDegree > 0.0001)
					_turningDegree -= (PI / 4.0f) * _deltaTime;
				else if (_turningDegree < -0.0001)
					_turningDegree += (PI / 4.0f) * _deltaTime;
				else
					_turningDegree = 0.0f;
			}
		}
	}
	break;
	case INPUT_TYPES::TRIGGERS:
	{
		/* Update whether the player is drifting. */
		_wasDrifting = _isDrifting;
		_isDrifting = (X > 0 && Y > 0 && _turningDegree != 0);

		/*if (X != 0)
		{
			
		}*/

		if (Y != 0)
		{
			/*SoundMessageContent *content1 = new SoundMessageContent;
			content1->name = "SFX03";
			content1->subType = S_TYPE::playSound;
			std::shared_ptr<Message> myMessage1 = std::make_shared<Message>(Message(MESSAGE_TYPE::SoundMessageType));
			myMessage1->setContent(content1);
			MessagingSystem::instance().postMessage(myMessage1);*/

			SoundMessageContent *content2 = new SoundMessageContent;
			content2->name = "SFX04";
			content2->subType = S_TYPE::playSound;
			std::shared_ptr<Message> myMessage2 = std::make_shared<Message>(Message(MESSAGE_TYPE::SoundMessageType));
			myMessage2->setContent(content2);
			MessagingSystem::instance().postMessage(myMessage2);
		}

		/* Update camera turning. */
		if (!cameraIndependant)
		{
			if (_lookBack)
				_camera_p->_transform.rotateAround(_playerToCamera, _player_p->_transform._position, _player_p->_transform._orientation * MakeQFromEulerAngles(Vector3(0.0f, PI, 0.0f)));
			else 
				_camera_p->_transform.rotateAround(_playerToCamera, _player_p->_transform._position, _player_p->_transform._orientation * MakeQFromEulerAngles(Vector3(0.0f, _lookAngle, 0.0f)));

			GLfloat angleY = atan2((_camera_p->_transform._position.z - _player_p->_transform._position.z), (_camera_p->_transform._position.x - _player_p->_transform._position.x));
			_camera_p->_transform._orientation = MakeQFromEulerAngles(Vector3(0.0f, angleY - PI / 2.0f, 0.0f));
		}

		/* Send physics message */
		PhysicsAccelerateContent *content = new PhysicsAccelerateContent();
		content->amountFast = Y;
		content->amountSlow = X;
		content->turningDegree = _turningDegree;
		content->_isDrifting = _isDrifting;
		content->_wasDrifting = _wasDrifting;
		content->object = _player_p;
		std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsAccelerateCallType, false));
		inputMessage->setContent(content);
		MessagingSystem::instance().postMessage(inputMessage);
	}
	break;
	default:
		break;
	}
}

void InputEngine::checkAxis(SDL_GameControllerAxis x, SDL_GameControllerAxis y, INPUT_TYPES type) 
{
	int16_t degreeX = SDL_GameControllerGetAxis(gameController, x);
	int16_t degreeY = SDL_GameControllerGetAxis(gameController, y);
	if ((degreeX < CONTROLLER_DEADZONE && degreeX > -CONTROLLER_DEADZONE) && !(degreeX > CONTROLLER_DEADZONE || degreeX < -CONTROLLER_DEADZONE))
		degreeX = 0;

	if ((degreeY < CONTROLLER_DEADZONE && degreeY > -CONTROLLER_DEADZONE) && !(degreeY > CONTROLLER_DEADZONE || degreeY < -CONTROLLER_DEADZONE))
		degreeY = 0;

	float xAmount = (float)degreeX / imax;
	float yAmount = (float)degreeY / imax;

	axisEventHandler(xAmount, yAmount, type);
}

void InputEngine::checkInput(GLfloat deltaTime)
{
	_deltaTime = deltaTime; 
	while (!_messageQueue.empty())
	{
		std::shared_ptr<Message> myMessage = _messageQueue.front();

		switch (myMessage->getType())
		{
			case MESSAGE_TYPE::InputButtonDownCallType:
			{
				InputButtonDownContent* content = static_cast<InputButtonDownContent*>(myMessage->getContent());
				buttonEventHandler(content->ev);
				_messageQueue.pop();
			}
		}
	}
	if (gameController != NULL) {
        checkAxis(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY, INPUT_TYPES::LOOK_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY, INPUT_TYPES::MOVE_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, INPUT_TYPES::TRIGGERS);
    }
}