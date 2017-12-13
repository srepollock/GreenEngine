<!--======================================================================================
    Messaging System Documentation
=======================================================================================-->
# Messaging System Documentation

_Copyright 2017 Erick Fernandez de Arteaga. All rights reserved._

- [Erick's LinkedIn Profile][ref_linkedin_profile]

- [Erick's Bitbucket Profile][ref_bitbucket_profile]

<br>

<!--======================================================================================
    Table of Contents
=======================================================================================-->
## Table of Contents
---

1. [The Big Rule](#section_big_rule)

1. [Defining a New Message Type](#section_new_type)

1. [Sending Messages](#section_sending)

1. [Receiving Messages](#section_receiving)

<br>

<!--======================================================================================
    The Big Rule
=======================================================================================-->
<a id="section_big_rule"></a>
## The Big Rule
---

_Every_ messages is _always_ a `std::shared_ptr`.

Whenever you create a message, create it as type `std::shared_ptr<Message>`.

No message should _ever_, even temporarily, be a stack object or non-shared pointer.

Being stored in a `std::shared_ptr` allows for messages to persists as long as any thread might still need them. Once all `std::shared_ptr`s to a given message have gone out of scope, the message will automatically be deleted. This means that messages should also _never_ be explicitly deleted.

<br>

<!--======================================================================================
    Defining a New Message Type
=======================================================================================-->
<a id="section_new_type"></a>
## Defining a New Message Type
---

The steps to define a new message type are as follows:

1. Add an item to the `MESSAGE_TYPE` enum in `MessageTypes.h`.
   
   ```cpp
   enum class MESSAGE_TYPE
   {
      // ...,
      // ...,
      // ...,
      MyMessageType
   }
   ```

1. Create a subclass of `BaseMessageContent` in `MessageTypes.h`. The subclass should inherit publicly and contain fields for all of the data you want the message to contain. If any of the fields are pointers, make sure they are deleted at some point.

   ```cpp
   class MyMessageContent : public BaseMessageContent
   {
      public:
         std::string someDataString;
   }
   ```

<br>

<!--======================================================================================
    Sending Messages
=======================================================================================-->
<a id="section_sending"></a>
## Sending Messages
---

To send a message, you must first create it and set its content.

```cpp
std::shared_ptr<Message> message = std::make_shared<Message>(MESSAGE_TYPE::MyMessageType, false);

MyMessageContent* content = new MyMessageContent();

content->someDataString = "This is the content of the message.";

message->setContent(content);

MessagingSystem::instance().postMessage(message);
```

The first line creates a message of type `MyMessageType` that is not urgent and leaves its content null. Note that the function `std::make_shared<T>()` is used instead of the `std::shared_ptr<T>` constructor. This is strongly preferred for exception safety and efficiency. Note also that the `Message` constructor is _not_ explicitly called with `new`. Instead, the arguments to the constructor are passed as args to `std::make_shared<T>`.

The second line creates the message content object. This goes without saying, but the content type should _always_ match the message type. Note that the keyword `new` _is_ used here.

The third line sets a field in the content. This is the data that the message actually conveys. The fields to set will depend on the message content type.

The fourth line sets the message content to the content object that was created.

The fifth line posts the message to the messaging system.

Lastly, any method that sends messages should exit regularly so that the `std::shared_ptr`s created in the method have a chance to go out of scope. Otherwise the messages will never be deleted!

<br>

<!--======================================================================================
    Receiving Messages
=======================================================================================-->
<a id="section_receiving"></a>
## Receiving Messages
---

To receive messages, your object must inherit publicly from `MessageReceiver`, subscribe to the appropriate message type, and use its `messageHandler()` method.

Inheriting from MessageReceiver:

```cpp
class MyReceiverClass : public MessageReceiver { ... };
```

Subscribing to a message type:

```cpp
MyReceiverClass myReceiver = new myReceiver();

myReceiver.subscribe(MESSAGE_TYPE::MyMessageType);
```

You could also subscribe in your constructor:

```cpp
MyReceiverClass::MyReceiverClass()
{
   subscribe(MESSAGE_TYPE::MyMessageType);
}
```

Once subscribed, messages will be sent to your object's `messageHandler()` method. You can override this method and do whatever you'd like with the message. Keep in mind, however, that the MessagingSystem will be waiting on your object for the duration of the MessageHandler method, so try to keep it relatively quick.

The `messageHandler()` method returns a bool. This should almost always be false. If you return true, the messaging system will not send this message out to any other object that has subscribed to it.

```cpp
bool messageHandler(std::shared_ptr<Message> message)
{
   // Do something with the message.
   return false;
}
```

To access the message's content, you must cast it to the correct type based on the message type as show below:

```cpp
bool messageHandler(std::shared_ptr<Message> message)
{
   switch (message->getType())
   {
      case MESSAGE_TYPE::MyMessageType:
         MyMessageContent* content = static_cast<MyMessageContent*>(message->getContent());
         std::cout << _content->someDataString;
         break;
      // Do other things for other message types.
   }
   
   return false;
}
```

If you need to do something more lengthy with a message but don't want to hold up the messaging system, you can make use of the default implementation of `messageHandler()`. The default implementation places urgent messages in the `_urgentMessageQueue` and non-urgent messages in the `_messageQueue`. You can then process these messages on your own time in some other method elsewhere.

If you choose to do this, keep in mind that you should `pop()` each message from its queue after processing it so that it can be deleted. Also keep in mind that messages might arrive at a queue while you are modifying it, so you must lock the `_messageQueueMutex_p` and/or `_urgentMessageQueueMutex_p` when accessing or modifying the queues to prevent conflicts.

A basic example of a method that processes queued messages is provided below. Feel free to use this as a template.

```cpp
///
///   Processes a single queued message.
///   If there is at least one urgent message, 
///   an urgent message will be processed.
///   If there are no urgent messages, 
///   a regular message will be processed.
///
void ProcessQueuedMessage()
{
   _messageQueueMutex_p->lock();
   _urgentMessageQueueMutex_p->lock();

   /* Do nothing if there are no messages. */
   if (_messageQueue.empty() && _urgentMessageQueueMutex.empty())
   {
      _messageQueueMutex_p->unlock();
      _urgentMessageQueueMutex_p->unlock();
   }
   /* If there are urgent messages, process one. */
   else if (!_urgentMessageQueueMutex.empty())
   {
      _messageQueueMutex_p->unlock();

      std::shared_ptr<Message> message = _urgentMessageQueue.front();

      // Do something with the message.

      _urgentMessageQueue.pop();
      _urgentMessageQueueMutex_p->unlock();
   }
   /* If there are no urgent messages, process a normal message. */
   else
   {
      _urgentMessageQueueMutex_p->unlock();

      std::shared_ptr<Message> message = _messageQueue.front();

      // Do something with the message.
      
      _messageQueue.pop();
      _messageQueueMutex_p->unlock();
   }
}
```

<br>

<!--======================================================================================
    Reference Links
=======================================================================================-->
[ref_linkedin_profile]: https://www.linkedin.com/in/erick-fda "Erick's LinkedIn Profile"

[ref_bitbucket_profile]: https://bitbucket.org/erick-fda "Erick's Bitbucket Profile"
