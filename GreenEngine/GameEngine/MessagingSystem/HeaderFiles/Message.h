/*===================================================================================*//**
	Message
	
	A message to be sent by the MessagingSystem.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see Message
	@see Message.cpp
	
*//*====================================================================================*/

#ifndef MESSAGE_H
#define MESSAGE_H
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

/*========================================================================================
	Dependencies
========================================================================================*/
#include "MessageTypes.h"

/*========================================================================================
	Message	
========================================================================================*/
/**
	The base class for all messages to be sent by the MessagingSystem.
	Specific message classes should inherit from this one.
	
	@see Message
	@see Message.cpp
*/
class Message
{
	/*------------------------------------------------------------------------------------
		Class Fields
	------------------------------------------------------------------------------------*/
	private:

    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		MESSAGE_TYPE _type;
		bool _isUrgent;
		BaseMessageContent* _content_p;


    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		Message(MESSAGE_TYPE type = MESSAGE_TYPE::BaseMessageType, bool isUrgent = false);
        ~Message();

	/*------------------------------------------------------------------------------------
		Instance Getter Methods
    ------------------------------------------------------------------------------------*/
    public:
		MESSAGE_TYPE getType();
		bool getIsUrgent();
		BaseMessageContent* getContent();
    
	/*------------------------------------------------------------------------------------
		Instance Setter Methods
	------------------------------------------------------------------------------------*/
    public:
		void setContent(BaseMessageContent* newContent);

	/*------------------------------------------------------------------------------------
		Instance Methods
	------------------------------------------------------------------------------------*/
    public:


    private:

};

#endif
