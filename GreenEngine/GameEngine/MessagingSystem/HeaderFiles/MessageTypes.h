/*===================================================================================*//**
	MessageTypes
	
	Header file for message and message content types.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see MessageTypes
	@see MessageTypes.cpp
	
*//*====================================================================================*/

#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

/*========================================================================================
	Dependencies
========================================================================================*/

#include "../../RenderEngine/HeaderFiles/RenderableTypes.h" //should probably fix that path
#include "../../Components/HeaderFiles/GameObject.h"
#include "../../HeaderFiles/InputTypes.h"
#include <string>
#include <memory>

/*========================================================================================
	Enums
========================================================================================*/
/**
	Enum of message types.
	
	-	Naming convention for message types is [Message type] + "Type".
		For example, "BaseMessage" has type "BaseMessageType".
*/
enum class MESSAGE_TYPE
{
	BaseMessageType,
	PhysicsCallMessageType,
	PhysicsInitializeCallType,
	FileLoadMessageType,
	InputMessageType,
	FileLoadImageMessageType,
	FileLoadedMessageType,
	FileLoadedImageMessageType,
	RenderLoadMessageType,
	RenderReadyMessageType,
	RenderLoadSingleMessageType,
	RenderDrawMessageType,
	RenderDrawOverlayMessageType,
	RenderUnloadMessageType,
	RenderFinishedMessageType
};

/*========================================================================================
	Message Content Types
========================================================================================*/
/**
	Each type of message should have a small class for its content.
	
	-	Naming convention for content types is [Message type] + "Content".
		For example, "Base Message" has content type "BaseMessageContent".
	
	-	All content variables should be public.

	-	Setting a message's content deletes the old content's memory! Because of this, 
		messages should not share pointers to the same content object!
*/

class BaseMessageContent
{
	public:
		///
		///	Virtual destructor allows for derived content types to be deleted via
		///	a BaseMessageContent*.
		///
		virtual ~BaseMessageContent() {};
};

//*****RENDERER MESSAGES

class RenderLoadMessageContent : public BaseMessageContent
{
	public:
		RenderableSetupData data;
};

class RenderReadyMessageContent : public BaseMessageContent
{

};

class RenderLoadSingleMessageContent : public BaseMessageContent
{
	public:
		RenderableModel model;
		RenderableTexture texture;
};

class RenderDrawMessageContent : public BaseMessageContent
{
	public:
		RenderableScene *scene_p; //renderer will ALWAYS be responsible for deletion
};

class RenderDrawOverlayMessageContent : public BaseMessageContent
{
	public:
		RenderableOverlay *overlay_p; //renderer will ALWAYS be responsible for deletion
};

class RenderUnloadMessageContent : public BaseMessageContent
{

};

class RenderFinishedMessageContent : public BaseMessageContent
{
	
};

//*****FILE MESSAGES

class PhysicsCallMessageContent: public BaseMessageContent
{
	public:
	//std::vector<std::shared_ptr<GameObject>> _objectsToUpdate;
	std::string contentVar;
	std::map<std::string, GameObject*> worldObjects;
	float_t deltaTime;
	PhysicsCallMessageContent(std::string s) { contentVar = s; }
};

class PhysicsInitializeContent : public BaseMessageContent
{
	public:
		GameObject* camera;
		GameObject* player;
};

class FileLoadMessageContent : public BaseMessageContent
{
public:
	std::string path;
	bool relative;
};

class FileLoadImageMessageContent : public BaseMessageContent
{
public:
	std::string path;
	bool relative;
};

class FileLoadedMessageContent : public BaseMessageContent
{
public:
	size_t hash;
	std::string path;
	bool relative;
	std::string content;
};

class FileLoadedImageMessageContent : public BaseMessageContent
{
public:
	size_t hash;
	std::string path;
	bool relative;
	std::shared_ptr<SDL_Surface> image;
};

class InputMessageContent : public BaseMessageContent
{
public:
	INPUT_TYPES type;
	float lookX, lookY;
};

#endif
