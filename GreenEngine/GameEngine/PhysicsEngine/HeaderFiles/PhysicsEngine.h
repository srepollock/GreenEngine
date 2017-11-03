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
#include "GameObject.h"
#include "../../HeaderFiles/InputTypes.h"
#include "../../Components/HeaderFiles/ComponentHeaders.h"

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
		GameObject* _player_p;
		GameObject* _camera_p;
		GLfloat _deltaTime;
		const float MATH_PI = 3.14159;

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
			void translate(GameObject *go, Vector3 translation);
			void translate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
			void translateForward(GameObject *go, Vector3 traslation);
			void accelerate(GameObject *go, Vector3 amount);
			void accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
			void decelerate(GameObject *go, Vector3 amount);
			void decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z);
			void rotate(GameObject *go, Vector3 amount);
			void rotateX(GameObject *go, GLfloat angle);
			void rotateY(GameObject *go, GLfloat angle);
			void rotateZ(GameObject *go, GLfloat angle);
	#pragma endregion

    private:
		void loop();
		void checkMessage(std::shared_ptr<Message>);
		void getControllerInput(InputMessageContent*);
		void applyAcceleration(GameObject*);
		void generalPhysicsCall(GameObject*);
};