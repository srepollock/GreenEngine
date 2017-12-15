#include "../HeaderFiles/Scene.h"

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
			rc.rotation = Vector3ToGLMVector(it->second->_transform._orientation.MakeEulerAnglesFromQ());
			rc.viewAngle = cc->getAngle();
			rs->camera = rc;
		}			
		else 
		{
			if (it->second->hasComponent<LightComponent*>()) {
				RenderableLight rl;
				LightComponent *lc = it->second->getComponent<LightComponent*>();
				rl.type = lc->_type;
				rl.intensity = lc->_intensity;
				rl.color = Vector3ToGLMVector(lc->_color);
				rl.angle = lc->_angle;
				rl.range = lc->_range;
				rl.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				rl.rotation = Vector3ToGLMVector(it->second->_transform._orientation.MakeEulerAnglesFromQ());
				rl.scale = FloatToGLMVector(it->second->_transform.getScale());
				rs->lights.push_back(rl);
			}

			if (it->second->hasComponent<RenderComponent*>()) {
				RenderableObject ro;
				RenderComponent *rc = it->second->getComponent<RenderComponent *>();
				ro.albedoName = rc->getAlbedoName();
				ro.normalName = rc->getNormalName();
				ro.smoothness = rc->getSmoothness();
				ro.modelName = rc->getModelName();
				ro.position = Vector3ToGLMVector(it->second->_transform.getPosition());
				ro.rotation = Vector3ToGLMVector(it->second->_transform._orientation.MakeEulerAnglesFromQ());
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
	GameObject *go = new GameObject(new Transform(new Vector3(0, 4, -6.5), new Vector3(0, 0, 0), 1.0f));
	go->addComponent(new CameraComponent(new Vector3(0.5,0.5,0.5), 0.1f, 1000.0f, 1.05f));
	addGameObject("Camera", go);

	InputInitializeContent* content = new InputInitializeContent(); 
	content->camera = go;

	go = new GameObject(new Transform(new Vector3(0, 0.5f, 0), new Vector3(0, 0, 0), 1.0f), "player");
	go->addComponent(new RenderComponent("car2_body", "car2_base", "", 0.75f));
	go->addComponent(new BoxColliderComponent("car2_body"));
	go->addComponent(new RigidBodyComponent(2.5f, 60.0f, 1850.0f, 0.0f, 0.0f, 0.0f, Vector3(1,1,2)));
	GameObject *childWheelFL, *childWheelFR, *childWheelRL, *childWheelRR;
	childWheelFL = new GameObject(new Transform(new Vector3(1.2f, 0.25f, 2.4f), new Vector3(0.0f, 0.0f, 0.0f), 1.0f), "wheelFL");
	childWheelFL->addComponent(new RenderComponent("car2_wheel", "car2_wheel", "", 1.0f));
	childWheelFR = new GameObject(new Transform(new Vector3(-1.2f, 0.25f, 2.4f), new Vector3(0.0f, 0.0f, 0.0f), 1.0f), "wheelFR");
	childWheelFR->addComponent(new RenderComponent("car2_wheel", "car2_wheel", "", 1.0f));
	go->addChild(childWheelFL);
	go->addChild(childWheelFR);
	childWheelRL = new GameObject(new Transform(new Vector3(1.2f, 0.25f, -1.4f), new Vector3(0.0f, 0.0f, 0.0f), 1.0f), "wheelRL");
	childWheelRL->addComponent(new RenderComponent("car2_wheel", "car2_wheel", "", 1.0f));
	childWheelRR = new GameObject(new Transform(new Vector3(-1.2f, 0.25f, -1.4f), new Vector3(0.0f, 0.0f, 0.0f), 1.0f), "wheelRR");
	childWheelRR->addComponent(new RenderComponent("car2_wheel", "car2_wheel", "", 1.0f));
	go->addChild(childWheelRL);
	go->addChild(childWheelRR);

	// Forward directions for object
	GameObject *forward = new GameObject(new Transform(new Vector3(go->_transform._position + go->_transform._forward), new Vector3(PI / 2, 0, 0), 0.25f), "forward");
	GameObject *right = new GameObject(new Transform(new Vector3(go->_transform._position + go->_transform._right), new Vector3(0, 0, -PI / 2), 0.25f), "right");
	GameObject *up = new GameObject(new Transform(new Vector3(go->_transform._position + go->_transform._up), new Vector3(0, 0, 0), 0.25f), "up");
	forward->addComponent(new RenderComponent("cone", "test_texture", "", 0.0f));
	forward->addComponent(new LightComponent(1.5f, new Vector3(0.0f, 1.0f, 0.75f), 20.0f, 1.5f, RenderableLightType::POINT));
	right->addComponent(new RenderComponent("cone", "test_texture2", "", 0.0f));
	up->addComponent(new RenderComponent("cone", "rainbow", "", 0.0f));
	go->addChild(forward);
	go->addChild(up);
	go->addChild(right);
	addGameObject("Player", go);  
	addGameObject("Player.WheelFL", childWheelFL);
	addGameObject("Player.WheelFR", childWheelFR);
	addGameObject("Player.WheelRL", childWheelRL);
	addGameObject("Player.WheelRR", childWheelRR);
	addGameObject("Player.ChildF", forward);
	addGameObject("Player.childR", right);
	addGameObject("Player.childU", up);

	content->player = go;
	std::shared_ptr<Message> msg = std::make_shared<Message>(Message(MESSAGE_TYPE::InputInitializeCallType, false));
	msg->setContent(content);
	MessagingSystem::instance().postMessage(msg);

	/*go = new GameObject(new Transform(new Vector3(0, 0, 0), new Vector3(0, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("raceTrack", "test_texture2", "", 0));
	addGameObject("Road", go);*/

	go = new GameObject(new Transform(new Vector3(-5, 2, 10), new Vector3(0, 0, 0), 1.0f), "Cube");
	go->addComponent(new RenderComponent("cube", "test_texture3", "", 0));
	go->addComponent(new RigidBodyComponent(2.5f, 60.0f, 1850.0f, 0.0f, 0.0f, 0.0f, Vector3(1, 1, 1)));
	go->addComponent(new BoxColliderComponent("cube"));
	addGameObject("Cube", go);
	
	go = new GameObject(new Transform(new Vector3(0, 0, 0), new Vector3(0, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("track2a", "grass2_d", "grass2_n", 0.5f));
	addGameObject("Ground", go);

	go = new GameObject(new Transform(new Vector3(0, -2.9f, 0), new Vector3(0, 0, 0), 3.0f));
	go->addComponent(new RenderComponent("track3b", "asphalt", "", 0.25f));
	addGameObject("Road", go);

	go = new GameObject();
	go->addComponent(new LightComponent(0.5f, new Vector3(1, 1, 1), 0.0f, 0.0f, RenderableLightType::AMBIENT));
	addGameObject("LightAmb", go);

	go = new GameObject(new Transform(new Vector3(0, 50.0f, 0), new Vector3(1.25f, 0, 0), 1.0f));
	//go->addComponent(new RenderComponent("cube", "crate", "", 0)); 
	go->addComponent(new LightComponent(0.75f, new Vector3(1.0f, 0.9f, 0.85f), 1000.0f, 0.0f, RenderableLightType::DIRECTIONAL)); 
	addGameObject("LightDir", go);

	go = new GameObject( new Transform(new Vector3(3.0f, 5.0f, 3.0f), new Vector3(0,0,0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	go->addComponent(new LightComponent(2.0f, new Vector3(0.0,0.0, 0.0), 20.0f, 0.0f, RenderableLightType::POINT));
	addGameObject("Light2", go);

	go = new GameObject(new Transform(new Vector3(-5.0f, 2.0f, 0.0f), new Vector3(0, 1.5f, 0), 1.0f));
	go->addComponent(new RenderComponent("cube", "crate", "", 0));
	go->addComponent(new LightComponent(5.0f, new Vector3(1.0f, 1.0f, 1.0f), 15.0f, 0.6f, RenderableLightType::SPOT));
	addGameObject("Light3", go);
	std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::SceneDoneLoadType));
	MessagingSystem::instance().postMessage(myMessage);
};

