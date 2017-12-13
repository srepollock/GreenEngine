#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif
// TODO: These are relative with the build, the paths can be removed
#include "../RenderEngine/HeaderFiles/RenderableTypes.h"
#include "../Components/HeaderFiles/GameObject.h"
#include "../Components/HeaderFiles/Transform.h"
#include "../MessagingSystem/HeaderFiles/MessageReceiver.h"
#include "../../HeaderFiles/InputTypes.h"
#include "../MessagingSystem/HeaderFiles/MessagingSystem.h"
#include <vector>
#include <mutex>

class Scene
{
public:
	Scene();
	~Scene();
	std::map<std::string, GameObject*> _worldObjects;
	void addGameObject(std::string, GameObject*);
	void deleteGameObject(std::string);
	GameObject* getGameObject(std::string);
	RenderableScene* getRenderInformation();
	std::map<std::string, GameObject*> getWorldObjects();
	glm::vec3 Vector3ToGLMVector(Vector3 vec);
	glm::vec3 FloatToGLMVector(GLfloat num);
	std::mutex _worldMutex;
private:
	void setUpSceneOne();

};