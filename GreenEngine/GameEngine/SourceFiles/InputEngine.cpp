#include "InputEngine.h"

InputEngine::InputEngine() {
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

InputEngine::~InputEngine() {
	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}
}



void InputEngine::buttonEventHandler(SDL_Event ev)
{
	InputMessageContent *inputContent = new InputMessageContent();
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_Y:
			inputContent->type = INPUT_TYPES::Y_BUTTON;
			break;
			
			break;
		default:
			break;
	}

	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType, false));
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

void InputEngine::axisEventHandler(GLfloat X, GLfloat Y, INPUT_TYPES type)
{
	InputMessageContent *inputContent = new InputMessageContent();
	inputContent->type = type;
	inputContent->lookX = X;
	inputContent->lookY = Y;

	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType));
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

void InputEngine::checkAxis(SDL_GameControllerAxis x, SDL_GameControllerAxis y, INPUT_TYPES type) {
	int16_t degreeX = SDL_GameControllerGetAxis(gameController, x);
	int16_t degreeY = SDL_GameControllerGetAxis(gameController, y);
	if ((degreeX < CONTROLLER_DEADZONE && degreeX > -CONTROLLER_DEADZONE) && !(degreeX > CONTROLLER_DEADZONE || degreeX < -CONTROLLER_DEADZONE))
		degreeX = 0;

	if ((degreeY < CONTROLLER_DEADZONE && degreeY > -CONTROLLER_DEADZONE) && !(degreeY > CONTROLLER_DEADZONE || degreeY < -CONTROLLER_DEADZONE))
		degreeY = 0;

	if (degreeX != 0 || degreeY != 0)
		axisEventHandler((float)degreeX / imax, (float)degreeY / imax, type);
	
}

void InputEngine::checkInput()
{
	if (gameController != NULL) {
        checkAxis(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY, INPUT_TYPES::LOOK_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY, INPUT_TYPES::MOVE_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, INPUT_TYPES::TRIGGERS);
    }
}