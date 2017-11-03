/*===================================================================================*//**
	MessagingSystem
	
	Singleton for sending messages to objects that have subscribed to particular message 
    types.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see MessagingSystem
	@see MessagingSystem.cpp
	
*//*====================================================================================*/

#ifndef MESSAGING_SYSTEM_H
#define MESSAGING_SYSTEM_H

/*========================================================================================
	Dependencies
========================================================================================*/
#include <functional>
#include <thread>
#include <map>
#include <mutex>
#include <queue>
#include "Message.h"
#include "MessageReceiver.h"
using ReceiverGroup = std::vector<MessageReceiver*>;

/*========================================================================================
	MessagingSystem	
========================================================================================*/
/**
	Singleton for sending messages to objects that have subscribed to particular message 
    types.
	
	@see MessagingSystem
	@see MessagingSystem.cpp
*/
class MessagingSystem
{
    /*------------------------------------------------------------------------------------
		Singleton
    ------------------------------------------------------------------------------------*/
    public:
        /* Ensure copy constructor and assignment operator for singleton are not implemented. */
		MessagingSystem(MessagingSystem const&) = delete;
        void operator=(MessagingSystem const&) = delete;

        static MessagingSystem& instance();

	private:
		MessagingSystem();

    /*------------------------------------------------------------------------------------
		Instance Fields
    ------------------------------------------------------------------------------------*/
    private:
		bool _isDead;
		std::map <MESSAGE_TYPE, ReceiverGroup> _receiverGroups;
		std::queue<std::shared_ptr<Message>> _messageQueue;
		std::mutex _receiverGroupsMutex;
		std::mutex _messageQueueMutex;

    /*------------------------------------------------------------------------------------
		Constructors and Destructors
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
		void subscribe(MESSAGE_TYPE messageType, MessageReceiver* subscriberToAdd_p);
		void unsubscribe(MESSAGE_TYPE messageType, MessageReceiver* subscriberToRemove_p);
		void postMessage(std::shared_ptr<Message> messageToPost_p);
		void kill();

    private:
		void loop();
		void update();
		void sendMessage(std::shared_ptr<Message> messageToSend_p);
};

#endif
