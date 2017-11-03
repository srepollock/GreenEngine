#pragma once
#include <vector>
#include <mutex>
#include "../RenderEngine/HeaderFiles/RenderableTypes.h"
#include "../Components/HeaderFiles/GameObject.h"
#include "../Components/HeaderFiles/Transform.h"
#include "../MessagingSystem/HeaderFiles/MessageReceiver.h"
#include "../../HeaderFiles/InputTypes.h"
#include "../MessagingSystem/HeaderFiles/MessagingSystem.h"


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
	std::mutex _objectsMutex;
	std::unique_lock<std::mutex> lock1;
private:
	void setUpSceneOne();

};