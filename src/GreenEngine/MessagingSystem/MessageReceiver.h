/*===================================================================================*//**
	MessageReceiver
	
	Superclass for all classes that will subscribe to messaging system messages.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see MessageReceiver
	@see MessageReceiver.cpp
	
*//*====================================================================================*/

#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

/*========================================================================================
	Dependencies
========================================================================================*/
#include "Message.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

/*========================================================================================
	MessageReceiver	
========================================================================================*/
/**
	Superclass for all classes that will subscribe to messaging system messages.
	
	@see MessageReceiver
	@see MessageReceiver.cpp
*/
class MessageReceiver
{
    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    protected:
		bool _isDead;
		std::vector<MESSAGE_TYPE> _subscriptions;
		std::queue<std::shared_ptr<Message>> _messageQueue;
		std::queue<std::shared_ptr<Message>> _urgentMessageQueue;
		std::mutex* _subscriptionsMutex_p;
		std::mutex* _messageQueueMutex_p;
		std::mutex* _urgentMessageQueueMutex_p;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
    ------------------------------------------------------------------------------------*/
    public:
		MessageReceiver();
		~MessageReceiver();

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
		virtual bool messageHandler(std::shared_ptr<Message> message);
		void subscribe(MESSAGE_TYPE messageType);
		void unsubscribe(MESSAGE_TYPE messageType);

    protected:
		
};

#endif
