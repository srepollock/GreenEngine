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
	@see PhysicsEngine.cpp
	
*//*====================================================================================*/

#pragma once
#ifdef __APPLE__
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
#include <thread>
#include <math.h>
#include "MessageReceiver.h"
#include "../../MessagingSystem/HeaderFiles/MessagingSystem.h"
#include "MessageTypes.h"
#include "GameObject.h"
#include "../../HeaderFiles/InputTypes.h"
#include "../../Components/HeaderFiles/ComponentHeaders.h"

#define RHO 1.225
#define LINEARDRAGCOEF 0.5
#define ANGULARDRAGCOEF 0.05
#define GRAVITY -9.81


/*========================================================================================
	Dependencies
========================================================================================*/


/*========================================================================================
	PhysicsEngine	
========================================================================================*/
/**
	The physics engine for the RACE game engine.
	
	@see PhysicsEngine
	@see PhysicsEngine.cpp
*/
class PhysicsEngine : public MessageReceiver
{
    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		bool _running = false;
		GLfloat _deltaTime;
		const float MATH_PI = 3.14159f;
		float count = 0;
		float rotationAccel = 0;
		float _driftTimer = 0;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		PhysicsEngine();

		~PhysicsEngine();

	/*------------------------------------------------------------------------------------
		Instance Getter Methods
    ------------------------------------------------------------------------------------*/
    public:
        
    
	/*------------------------------------------------------------------------------------
		Instance Setter Methods
	------------------------------------------------------------------------------------*/
    public:


	/*------------------------------------------------------------------------------------
		Instance Methods
	------------------------------------------------------------------------------------*/
    public:
			std::thread* start();
			void stop();
			void flagLoop();
			// TODO: Physics function calls
	#pragma region Physics Calculation Methods
			void linearAccelerate(GameObject * obj, RigidBodyComponent * rbc);
			void angularAccelerate(RigidBodyComponent * rbc);
			void accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
	#pragma endregion

    private:
		void loop();
		void checkMessage(std::shared_ptr<Message>);
		void generalPhysicsCall(GameObject*);
		void applyAcceleration(GameObject * go, RigidBodyComponent * rc);
		void adjustForces(GameObject * go, RigidBodyComponent * rc);
		Vector3 getAngleFromTurn(GameObject *go, GLfloat tireDegree);
		void turnGameObject(GameObject *go);
		void collisionDetection(std::map<std::string, GameObject*> worldObj, GameObject *go);
		bool checkForCollision(GameObject *coll1, GameObject *coll2);
};