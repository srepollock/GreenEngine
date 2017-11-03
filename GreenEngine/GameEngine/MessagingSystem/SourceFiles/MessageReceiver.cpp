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
	@see MessageReceiver.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "MessageReceiver.h"
#include "MessagingSystem.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	MessageReceiver constructor.
///
MessageReceiver::MessageReceiver():
	_isDead(false), _subscriptionsMutex_p(new std::mutex()),
	_messageQueueMutex_p(new std::mutex()), _urgentMessageQueueMutex_p(new std::mutex())
{}

///
///	Default MessageReceiver destructor.
///
MessageReceiver::~MessageReceiver()
{
	/* Flag this message receiver as dead (it will ignore any new messages and subscriptions). */
	_isDead = true;

	_subscriptionsMutex_p->lock();

	/* Unsubscribe from all message subscriptions. */
	std::vector<MESSAGE_TYPE> subs = _subscriptions;
	for (MESSAGE_TYPE eachSubscription : subs)
	{
		_subscriptions.erase(remove(_subscriptions.begin(), _subscriptions.end(), eachSubscription), _subscriptions.end());
		MessagingSystem::instance().unsubscribe(eachSubscription, this);
	}

	_subscriptionsMutex_p->unlock();

	/* Delete pointers. */
	delete _subscriptionsMutex_p;
	delete _messageQueueMutex_p;
	delete _urgentMessageQueueMutex_p;
}

/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///	Handles incoming messages. Overridable for polymorphic behaviour by subclasses.
///
bool MessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	/* If this receiver is being destroyed, ignore messages. */
	if (_isDead)
	{
		return false;
	}

	/* Sort messages into the proper queue based on urgency. */
	if (message->getIsUrgent())
	{
		_urgentMessageQueueMutex_p->lock();
		_urgentMessageQueue.push(message);
		_urgentMessageQueueMutex_p->unlock();
	}
	else
	{
		_messageQueueMutex_p->lock();
		_messageQueue.push(message);
		_messageQueueMutex_p->unlock();
	}

	return false;
}

///
///	Subscribe to messages of the given type.
///
void MessageReceiver::subscribe(MESSAGE_TYPE messageType)
{
	/* If this receiver is being destroyed, ignore subscriptions. */
	if (_isDead)
	{
		return;
	}

	_subscriptionsMutex_p->lock();

	/* If the receiver is already subscribed to messages of the given type, return without doing anything. */
	for (MESSAGE_TYPE eachSubscription : _subscriptions)
	{
		if (eachSubscription == messageType)
		{
			_subscriptionsMutex_p->unlock();
			return;
		}
	}

	/* Subscribe to the message type. */
	_subscriptions.push_back(messageType);
	MessagingSystem::instance().subscribe(messageType, this);

	_subscriptionsMutex_p->unlock();
}

///
///	Unsubscribe from messages of the given type.
///
void MessageReceiver::unsubscribe(MESSAGE_TYPE messageType)
{
	/* If this receiver is being destroyed, ignore unsubscriptions. */
	if (_isDead)
	{
		return;
	}

	_subscriptionsMutex_p->lock();

	/* If this receiver is subscribed to messages of the given type, unsubscribe. */
	for (MESSAGE_TYPE eachSubscription : _subscriptions)
	{
		if (eachSubscription == messageType)
		{
			_subscriptions.erase(remove(_subscriptions.begin(), _subscriptions.end(), eachSubscription), _subscriptions.end());
			MessagingSystem::instance().unsubscribe(messageType, this);
			_subscriptionsMutex_p->unlock();
			return;
		}
	}

	_subscriptionsMutex_p->unlock();
}
