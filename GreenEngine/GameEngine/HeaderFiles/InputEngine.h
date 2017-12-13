#pragma once

#include "Message.h"
#include "MessagingSystem.h"
#include "MessageTypes.h"
#include "InputTypes.h"
#include <memory>
#include "SoundEngine.h"

#ifdef __APPLE__
#include "MacTypes.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <SDL.h>
#endif

class InputEngine : public MessageReceiver {
public:
    InputEngine();
    ~InputEngine();
	void buttonEventHandler(SDL_Event);
	void axisEventHandler(GLfloat, GLfloat, INPUT_TYPES);
	void checkAxis(SDL_GameControllerAxis, SDL_GameControllerAxis, INPUT_TYPES);
	void checkInput(GLfloat);
	void setUpInput();

private:
	int16_t _moveX, _moveY;
	int16_t _lookX, _lookY;
	const int CONTROLLER_DEADZONE = 8000;
	const int imax = std::numeric_limits<int16_t>::max();
	SDL_GameController *gameController;
	GameObject* _player_p;
	GameObject* _camera_p;
	bool cameraIndependant;
	Vector3 _playerToCamera;
	GLfloat _deltaTime;
	GLfloat _turningDegree = 0.0f;
	GLfloat _lookAngle = 0.0f;
	bool _isDrifting = false;
	bool _wasDrifting = false;
	bool _lookBack = false;

};