#include "../HeaderFiles/Scene.h"

const float MATH_PI = 3.14159;
Scene::Scene()
{
	setUpSceneOne();
}

Scene::~Scene()
{
	_worldObjects.clear();
	//_worldObjects.clear();
}

void Scene::addGameObject(std::string id, GameObject *obj)
{
	_worldObjects.emplace(id, obj);
}

void Scene::deleteGameObject(std::string id)
{
	_worldObjects.erase(_worldObjects.find(id));
}

GameObject* Scene::getGameObject(std::string id)
{
	return _worldObjects.find(id)->second;
}

std::map<std::string, GameObject*> Scene::getWorldObjects() {
	return _worldObjects;
}

RenderableScene* Scene::getRenderInformation()
{
	//_objectsMutex.lock();
	RenderableScene* rs = new RenderableScene();

	for (std::map<std::string, GameObject*>::iterator it = _worldObjects.begin(); it != _worldObjects.end(); ++it) {
		if (it->first == "Camera" && it->second->getComponent<CameraComponent*>() != nullptr) 
		{
			RenderableCamera rc;
			CameraComponent *cc = it->second->getComponent<CameraComponent*>();
			rc.clearColor = Vector3ToGLMVector(cc->getClearColor());
			rc.nearPlane = cc->getNearPlane();
			rc.farPlane = cc->getFarPlane();
			rc.position = Vector3ToGLMVector(it->second->_transform.getPosition());
			rc.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
			rc.viewAngle = cc->getAngle();
			rs->camera = rc;
		} else if (it->first == "Light") 
		{
			if (it->second->hasComponent<LightComponent*>()) {
				RenderableLight rl;
				LightComponent *lc = it->second->getComponent<LightComponent*>();
				rl.type = RenderableLightType::AMBIENT;
				rl.intensity = lc->_intensity;
				rl.color = Vector3ToGLMVector(lc->_color);
				rl.angle = lc->_angle;
				rl.range = lc->_range;
				rl.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				rl.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
				rl.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->lights.push_back(rl);
			}
		}
		else 
		{
			if (it->second->hasComponent<RenderComponent*>()) {
				RenderableObject ro;
				RenderComponent *rc = it->second->getComponent<RenderComponent *>();
				ro.albedoName = rc->getAlbedoName();
				ro.normalName = rc->getNormalName();
				ro.smoothness = rc->getSmoothness();
				ro.modelName = rc->getModelName();
				ro.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				ro.rotation = Vector3ToGLMVector(it->second->_transform.getRotation());
				ro.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->objects.push_back(ro);
			}
		}
	}
	//_objectsMutex.unlock();
	return rs;
};

glm::vec3 Scene::Vector3ToGLMVector(Vector3 vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
};

glm::vec3 Scene::FloatToGLMVector(GLfloat num) 
{
	return glm::vec3(num, num, num);
};

void Scene::setUpSceneOne() {
	GameObject *go = new GameObject(new Transform(new Vector3(0, 2, 10), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new CameraComponent(new Vector3(1,1,1), 0.1f, 1000.0f, 1.05f));
	addGameObject("Camera", go);

	PhysicsInitializeContent* content = new PhysicsInitializeContent();
	content->camera = go;

	go = new GameObject(new Transform(new Vector3(0, 2, 2), new Vector3(0, MATH_PI / 4, 0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	addGameObject("Cube", go);

	go = new GameObject(new Transform(new Vector3(5, 2.5, 0), new Vector3(0, 0, 0), 2.0f));
	go->addComponent(new RenderComponent("sphere", "rainbow", "", 0));
	addGameObject("Sphere", go);

	go = new GameObject(new Transform(new Vector3(0, 0.5f, 0), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new RenderComponent("carModel", "test_texture3", "", 0));
	go->addComponent(new AccelerationComponent(new Vector3(), 0.09f));
	go->addComponent(new VelocityComponent(new Vector3(), 0.01f));
	addGameObject("Player", go);

	content->player = go;
	std::shared_ptr<Message> msg = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsInitializeCallType, false));
	msg->setContent(content);
	MessagingSystem::instance().postMessage(msg);

	go = new GameObject(new Transform(new Vector3(0, 0, 0), new Vector3(0, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("road_floor", "test_texture2", "", 0));
	addGameObject("Road", go);

	go = new GameObject(new Transform(new Vector3(0, 0, -15), new Vector3(-1.5 * MATH_PI, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("road_floor", "test_texture", "", 0));
	addGameObject("Road2", go);

	go = new GameObject();
	go->addComponent(new LightComponent(0.5f, new Vector3(1, 1, 1), 0.0f, 0.0f));
	addGameObject("Light", go);
};

