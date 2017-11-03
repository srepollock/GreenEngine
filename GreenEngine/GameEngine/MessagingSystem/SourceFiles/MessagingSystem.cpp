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
	@see MessagingSystem.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "MessagingSystem.h"

/*----------------------------------------------------------------------------------------
	Singleton
----------------------------------------------------------------------------------------*/
///
///	Default MessagingSystem constructor. This is made private so that only 
///	MessagingSystem can create itself.
///
MessagingSystem::MessagingSystem()
{
	_isDead = false;
}

/// <summary>
///	Creates and returns the MessagingSystem singleton instance.
/// </summary>
MessagingSystem& MessagingSystem::instance()
{
    static MessagingSystem _instance;
    return _instance;
}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///	<summary>Continuously sends out messages from the message queue as long as the 
///	Messaging System instance remains alive.</summary>
///
void MessagingSystem::loop()
{
	/* Loop while the instance is alive. */
	while (!_isDead)
	{
		_messageQueueMutex.lock();
		
		/* If there are no queued messages, yield. */
		if (_messageQueue.empty())
		{
			_messageQueueMutex.unlock();
			std::this_thread::yield();
		}
		/* If there are queued messages, send the next message. */
		else
		{
			_messageQueueMutex.unlock();
			update();
		}
	}
}

///
/// Sends out a single message from the message queue.
///
void MessagingSystem::update()
{
	_messageQueueMutex.lock();

	sendMessage(_messageQueue.front());
	_messageQueue.pop();

	_messageQueueMutex.unlock();
}

///
///<summary>Posts a message to the messaging system.</summary>
///<param name="messageToPost">The message to post.</param>
///
void MessagingSystem::postMessage(std::shared_ptr<Message> messageToPost_p)
{
	/* If the MessagingSystem is dead, ignore posted messages. */
	if (_isDead)
	{
		return;
	}

	/* If the message is urgent, skip the queue and send it immediately. */
	if (messageToPost_p->getIsUrgent())
	{
		sendMessage(messageToPost_p);
	}
	/* If the message is not urgent, add it to the queue. */
	else
	{
		_messageQueueMutex.lock();
		_messageQueue.push(messageToPost_p);
		_messageQueueMutex.unlock();
	}
}

///
///<summary>Sends a message to all subscribers.</summary>
///<param name="messageToSend">The message to send.</param>
///
void MessagingSystem::sendMessage(std::shared_ptr<Message> messageToSend_p)
{
	/* If the MessagingSystem is dead, don't send any new messages. */
	if (_isDead)
	{
		return;
	}

	_receiverGroupsMutex.lock();

	/* If there is no subscriber group for this message type, return without doing anything. */
	if (_receiverGroups.count(messageToSend_p->getType()) == 0)
	{
		_receiverGroupsMutex.unlock();
		return;
	}

	/* Get all subscribers for this message type and send the message to each of them. */
	ReceiverGroup& receivers = _receiverGroups[messageToSend_p->getType()];
	for (MessageReceiver* const eachReceiver_p : receivers)
	{
		/* Send the message to the next receiver. Stop sending out the message if indicated. */
		if (eachReceiver_p->messageHandler(messageToSend_p))
		{
			break;
		}
	}

	_receiverGroupsMutex.unlock();
}

///
///<summary>Begins the MessagingSystem loop on a new thread and returns a pointer to the thread.</summary>
///
std::thread* MessagingSystem::start()
{
	return new std::thread(&MessagingSystem::loop, this);
}

///
///<summary>Subscribes the given MessageReceiver to the given message type.</summary>
///<param name="messageType">The type of message to subscribe to.</param>
///<param name="subscriberToAdd">The MessageReceiver to add.</param>
///
void MessagingSystem::subscribe(MESSAGE_TYPE messageType, MessageReceiver* subscriberToAdd_p)
{
	/* If the MessagingSystem is dead, ignore new subscribers. */
	if (_isDead)
	{
		return;
	}

	_receiverGroupsMutex.lock();

	/* If there is no subscriber group for this message type yet, add one. */
	if (_receiverGroups.count(messageType) == 0)
	{
		_receiverGroups.insert(std::pair<MESSAGE_TYPE, ReceiverGroup>(messageType, ReceiverGroup()));
	}

	/* If the given subscriber is already subscribed to the given message type, return without doing anything. */
	ReceiverGroup& receivers = _receiverGroups[messageType];
	for (MessageReceiver* eachReceiver_p : receivers)
	{
		if (eachReceiver_p == subscriberToAdd_p)
		{
			_receiverGroupsMutex.unlock();
			return;
		}
	}

	/* Add the new subscriber. */
	receivers.push_back(subscriberToAdd_p);

	_receiverGroupsMutex.unlock();
}

///
///<summary>Unsubscribes the given MessageReceiver from messages of the given type.</summary>
///<param name="messageType">The type of message to unsubscribe from.</param>
///<param name="subscriberToRemove">The MessageReceiver to remove.</param>
///
void MessagingSystem::unsubscribe(MESSAGE_TYPE messageType, MessageReceiver* subscriberToRemove_p)
{
	/* If the MessagingSystem is dead, ignore unsubscriptions. */
	if (_isDead)
	{
		return;
	}

	_receiverGroupsMutex.lock();

	/* If there is no subscriber group for this message type, return without doing anything. */
	if (_receiverGroups.count(messageType) == 0)
	{
		_receiverGroupsMutex.unlock();
		return;
	}
	
	/* If the given subscriber is subscribed to the given message type, unsubscribe it. */
	ReceiverGroup& receivers = _receiverGroups[messageType];
	for (MessageReceiver* eachReceiver_p : receivers)
	{
		if (eachReceiver_p == subscriberToRemove_p)
		{
			receivers.erase(remove(receivers.begin(), receivers.end(), eachReceiver_p), receivers.end());
			_receiverGroupsMutex.unlock();
			return;
		}
	}

	_receiverGroupsMutex.unlock();
}

///
///	Kills the MessagingSystem.
/// Messages will stop being sent, and new subscribers will be ignored.
/// The MessagingSystem thread will exit the messaging loop and rejoin the thread that spawned it ASAP.
///
void MessagingSystem::kill()
{
	_isDead = true;
}
