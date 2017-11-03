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
#include <queue>
#include <atomic>

#include "RenderEngine.h"

#include "GlobalPrefs.h"
#include "FileEngine.h"
#include "OBJImport.h"
#include "main.h"

#include "RenderableTypes.h"
#include "RendererInternalTypes.h"
#include "RenderMessageReceiver.h"
#include "RenderFileMessageReceiver.h"
#include "RenderInputMessageReceiver.h"

#include "MessagingSystem.h"
#include "MessageTypes.h"
#include "MessageReceiver.h"
#include "Message.h"

#include "Shaders.h"
#include "Quad.h"
#include "Cube.h"

const int_least64_t IDLE_DELAY_CONST = 10;
const std::string MODEL_BASEPATH_CONST = "ResourceFiles/Models/";
const std::string TEXTURE_BASEPATH_CONST = "ResourceFiles/Textures/";
const std::string MODEL_EXTENSION_CONST = ".obj";
const std::string TEXTURE_EXTENSION_CONST = ".png";

class RenderEngineImplementation
{
public:
	RenderEngineImplementation()
	{
		//constructor: on engine thread
	}

	void start()
	{
		//start: on engine thread

		_window_p = g_window_p; //really ought to switch to dependency injection

		_state = RendererState::idle;

		//create message queue and handler, then subscribe to messaging
		_mq_p = new std::vector<std::shared_ptr<Message>>();
		_mqMutex_p = new std::mutex();
		_mr_p = new RenderMessageReceiver(_mq_p, _mqMutex_p);
		_mr_p->subscribeAll();
		
		//create file handling message queue and handler, then subscribe to messaging
		_fmq_p = new std::vector<std::shared_ptr<Message>>();
		_fmqMutex_p = new std::mutex();
		_fmr_p = new RenderFileMessageReceiver(_fmq_p, _fmqMutex_p);
		_fmr_p->subscribeAll();

		//create input message queue and handler, then subscribe to messaging
		_imq_p = new std::vector<std::shared_ptr<Message>>();
		_imqMutex_p = new std::mutex();
		_imr_p = new RenderInputMessageReceiver(_imq_p, _imqMutex_p);
		_imr_p->subscribeAll();
		
		//necessary protection on macOS
		_lastScene_p = nullptr;
		_lastOverlay_p = nullptr;

		//spawn thread
		_isRunning = true;
		_renderThread_p = new std::thread(&RenderEngineImplementation::loop, this);
	}

	void update()
	{
		//update: on engine thread, synchronized with game loop
	}

	~RenderEngineImplementation()
	{
		//destructor

		//kill render thread first
		_isRunning = false;
		_renderThread_p->join();
		delete(_renderThread_p);

		delete(_mqMutex_p);
		delete(_mr_p);
		delete(_mq_p);
		
		delete(_fmqMutex_p);
		delete(_fmr_p);
		delete(_fmq_p);

		delete(_imqMutex_p);
		delete(_imr_p);
		delete(_imq_p);
		
	}

private:

	//context and window
	SDL_Window *_window_p;
	SDL_GLContext _context_p;

	//state data
	std::atomic<RendererState> _state;
	RenderableScene *_lastScene_p;
	RenderableOverlay *_lastOverlay_p;
	RendererBuffer _buffer;

	//messaging stuff
	RenderMessageReceiver *_mr_p;
	RenderFileMessageReceiver *_fmr_p;
	RenderInputMessageReceiver *_imr_p;
	std::vector<std::shared_ptr<Message>> *_mq_p;
	std::vector<std::shared_ptr<Message>> *_fmq_p;
	std::vector<std::shared_ptr<Message>> *_imq_p;
	std::mutex *_mqMutex_p;
	std::mutex *_fmqMutex_p;
	std::mutex *_imqMutex_p;

	//resource lists
	std::map<std::string, ModelData> *_models_p;
	std::map<std::string, TextureData> *_textures_p;
	std::vector<ModelLoadingData> *_modelLoadQueue_p;
	std::vector<TextureLoadingData> *_textureLoadQueue_p;
	std::vector<ModelLoadingData> *_modelAwaitQueue_p;
	std::vector<TextureLoadingData> *_textureAwaitQueue_p;

	//threading stuff
	bool _isRunning;
	std::thread *_renderThread_p;

	//shader stuff
	GLuint _programID = 0;
	GLuint _shaderModelMatrixID = 0;
	GLuint _shaderMVPMatrixID = 0;
	GLuint _shaderTextureID = 0;
	GLuint _shaderSmoothnessID = 0;

	//framebuffer stuff
	int _renderWidth = 0;
	int _renderHeight = 0;

	//framebuffer textures
	GLuint _framebufferID = 0;
	GLuint _framebufferTexture0ID = 0;
	GLuint _framebufferTexture1ID = 0;
	GLuint _framebufferTexture2ID = 0;
	GLuint _framebufferDepthID = 0;

	//framebuffer program and uniforms
	GLuint _framebufferDrawProgramID = 0;
	GLuint _framebufferDrawVertexArrayID = 0;
	GLuint _framebufferDrawVertexBufferID = 0;
	GLuint _framebufferDrawTex0ID = 0;
	GLuint _framebufferDrawTex1ID = 0;
	GLuint _framebufferDrawTex2ID = 0;
	GLuint _framebufferDrawTex3ID = 0;
	GLuint _framebufferDrawBufferID = 0;

	//temporary cube stuff
	GLuint _cubeVertexArrayID = 0;
	GLuint _cubeVertexBufferID = 0;
	GLuint _cubeTextureID = 0;

	//base MVP, may keep or remove
	
	glm::mat4 _baseModelViewMatrix;
	glm::mat4 _baseModelViewProjectionMatrix;

	void loop()
	{
		//initial setup: run once

		SDL_Log("RenderEngine thread started!");

		setupStructuresOnThread();
		setupGLOnThread();
		setupSceneOnThread();

		//for testing
		_state = RendererState::idle;

		//loop: on RenderEngine thread
		while (_isRunning)
		{
			//TODO: state switching and stuff
			//doLoad, doRender/doImmediateLoad, doUnload

			checkQueue();

			switch ((RendererState)_state)
			{
			case RendererState::idle:
				//SDL_Log("Idle");
				doIdle();
				//std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_DELAY_CONST)); //don't busywait!
				break;
			case RendererState::loading:
				//SDL_Log("Loading");
				doLoad();
				break;
			case RendererState::rendering:
				//SDL_Log("Rendering");
				doSingleLoad();
				doRender(); //this should run really absurdly fast
				break;
			case RendererState::unloading:
				//SDL_Log("Unloading");
				doUnload();
				break;
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(17));
			SDL_GL_SwapWindow(_window_p);
		}

		//force unload/release if we're not already unloaded
		if (_state != RendererState::idle)
		{
			startUnload();
			doUnload();
		}

		//cleanup after run
		cleanupSceneOnThread();
		cleanupGLOnThread();
		cleanupStructuresOnThread();

		SDL_Log("RenderEngine thread halted!");
	}

	void setupStructuresOnThread()
	{
		//setup data structures

		_models_p = new std::map<std::string, ModelData>();
		_textures_p = new std::map<std::string, TextureData>();
		_modelLoadQueue_p = new std::vector<ModelLoadingData>();
		_textureLoadQueue_p = new std::vector<TextureLoadingData>();
		_modelAwaitQueue_p = new std::vector<ModelLoadingData>();
		_textureAwaitQueue_p = new std::vector<TextureLoadingData>();
	}

	void setupGLOnThread()
	{
		//this should work in theory as long as we don't need a GL context on the main thread
		//which would slow things down
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
		//SDL_GL_CreateContext(_window_p);
		_context_p = SDL_GL_CreateContext(_window_p); //we will need to modify this to release/acquire context in concert with the UI thread
		SDL_GL_SetSwapInterval(1);
		//SDL_GL_MakeCurrent(g_window_p, _context_p);
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			SDL_Log("Renderer: %s\n", glewGetErrorString(err));
		}
		SDL_Log((char*)glGetString(GL_VERSION));
		SDL_Log((char*)glGetString(GL_RENDERER));
	}

	void setupSceneOnThread()
	{
		setupWindow();
		setupProgram();
		setupFramebuffers();
		setupFramebufferDraw();
		//setupBaseMatrices(); //will need to move/redo to deal with moving camera
		setupCube(); //remove this soon
	}

	void cleanupSceneOnThread()
	{
		cleanupProgram();
		cleanupFramebuffers();
		cleanupFramebufferDraw();
		cleanupCube();
	}

	void cleanupGLOnThread()
	{
		//TODO cleanup all the GL gunk (necessary?)
	}

	void cleanupStructuresOnThread()
	{
		//delete data structures
		delete(_textureAwaitQueue_p);
		delete(_modelAwaitQueue_p);
		delete(_textureLoadQueue_p);
		delete(_modelLoadQueue_p);
		delete(_textures_p);
		delete(_models_p);
	}

	/// <summary>
	/// Checks the queue and grabs new state information
	/// </summary>
	void checkQueue()
	{
		//lock mutex...
		_mqMutex_p->lock();

		//needs to be sensitive to current state and prioritize certain messages
		//REMEMBER: the "head" of the queue is at 0 and the "tail" at the other end

		if (!_mq_p->empty()) //optimization: skip EVERYTHING if empty
		{
			if (_state == RendererState::idle)
			{
				auto iter = _mq_p->begin();

				//if nothing is loaded, wait for a load call and delete everything else
				for (; iter != _mq_p->end(); iter++)
				{
					std::shared_ptr<Message> msg_sp = *iter;
					if (msg_sp.get()->getType() == MESSAGE_TYPE::RenderLoadMessageType)
					{
						//great, we can load, so load and break!
						startLoad(&static_cast<RenderLoadMessageContent*>(msg_sp.get()->getContent())->data);
						iter = _mq_p->erase(iter); //don't forget to delete that!
						break;
					}
					else
					{
						//remove and log warning
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: found a message before load");
						//potential for leak here if it's a render message, but not the culprit in this case
						iter = _mq_p->erase(iter);
					}
				}
			}
			else if (_state == RendererState::loading)
			{
				//we won't do anything until the load is done- not even abort!
			}
			else if (_state == RendererState::rendering)
			{
				//if rendering:
				//	if we have an unload message, ignore other messages, start unload and purge everything before unload
				//	if we have a load message, push to "immediate load" queue and continue rendering as normal
				//  if we don't have either, simply do a render (grab latest render scene and render overlay messages)

				//algorithm:
				//traverse from start to finish:
				//store the latest render message and index
				//store the latest renderOverlay message and index
				//if we hit an unload instruction, store that and break
				//if we hit a loadsingle instruction, queue it and continue
				//if we hit a load instruction, log warning, start unload, store index, and break
				RenderableScene *latestScene = nullptr;
				int latestSceneIndex = -1;
				RenderableOverlay *latestOverlay = nullptr;
				int latestOverlayIndex = -1;
				bool forceAbort = false;
				int abortIndex = -1;

				for (int i = 0; i < _mq_p->size(); i++)
				{
					std::shared_ptr<Message> msg_sp = _mq_p->at(i);
					MESSAGE_TYPE t = msg_sp->getType();

					//DO NOT CHANGE THIS TO A SWITCH
					if (t == MESSAGE_TYPE::RenderDrawMessageType)
					{
						RenderableScene *scn = static_cast<RenderDrawMessageContent*>(msg_sp.get()->getContent())->scene_p;
						if (latestScene != nullptr)
							delete latestScene;
						latestScene = scn;
						latestSceneIndex = i;
						
					}
					else if (t == MESSAGE_TYPE::RenderDrawOverlayMessageType)
					{
						RenderableOverlay *ovl = static_cast<RenderDrawOverlayMessageContent*>(msg_sp.get()->getContent())->overlay_p;
						if (latestOverlay != nullptr)
							delete latestOverlay;
						latestOverlay = ovl;
						latestOverlayIndex = i;
					}
					else if (t == MESSAGE_TYPE::RenderUnloadMessageType)
					{
						abortIndex = i;
						forceAbort = true;
						break;
					}
					else if (t == MESSAGE_TYPE::RenderLoadSingleMessageType)
					{
						RenderLoadSingleMessageContent smc = *static_cast<RenderLoadSingleMessageContent*>(msg_sp.get()->getContent());
						if (!smc.model.name.empty())
						{
							ModelLoadingData mld;
							mld.name = smc.model.name;
							_modelLoadQueue_p->push_back(mld);
						}
						if (!smc.texture.name.empty())
						{
							TextureLoadingData tld;
							tld.name = smc.texture.name;
							_textureLoadQueue_p->push_back(tld);
						}
					}
					else if (t == MESSAGE_TYPE::RenderLoadMessageType)
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Received load message while scene is loaded!");
						abortIndex = i - 1;
						forceAbort = true;
						break;
					}
					else
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Bad message type received!");
					}
				}

				if (forceAbort)
				{
					if (abortIndex >= 0)
					{
						//if abortIndex is a thing, purge everything up and including abortIndex
						for (int i = 0; i < abortIndex; i++)
						{
							std::shared_ptr<Message> msg_sp = _mq_p->at(i);
							MESSAGE_TYPE t = msg_sp->getType();
							if (t == MESSAGE_TYPE::RenderDrawMessageType)
							{
								RenderableScene *scn = static_cast<RenderDrawMessageContent*>(msg_sp.get()->getContent())->scene_p;
								if (scn != nullptr)
									delete scn;
							}
							else if (t == MESSAGE_TYPE::RenderDrawOverlayMessageType)
							{
								RenderableOverlay *ovl = static_cast<RenderDrawOverlayMessageContent*>(msg_sp.get()->getContent())->overlay_p;
								if (ovl != nullptr)
									delete ovl;
							}
						}
						_mq_p->erase(_mq_p->begin(), _mq_p->begin() + abortIndex + 1);						
					}

					startUnload();
				}
				else
				{
					//if renderOverlay comes after render, delete it first, otherwise it'll get deleted normally
					//clear everything up to and including stored index
					if (latestOverlayIndex > latestSceneIndex)
					{
						_mq_p->erase(_mq_p->begin() + latestOverlayIndex);
					}
					_mq_p->erase(_mq_p->begin(), _mq_p->begin() + latestSceneIndex + 1);

					//assign renderablescene and renderableoverlay if they exist
					if (latestScene != nullptr)
					{
						if (_lastScene_p != nullptr)
							delete(_lastScene_p);
						_lastScene_p = latestScene;
					}
					if (latestOverlay != nullptr)
					{
						if (_lastOverlay_p != nullptr)
							delete(_lastOverlay_p);
						_lastOverlay_p = latestOverlay;
					}
				}

			}
			else if (_state == RendererState::unloading)
			{
				//like loading, we won't do anything if unloading
			}
			else
			{
				//well, that shouldn't happen
				SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Ended up in an impossible state");
			}
			
		}
		//if currently loading or unloading, ignore messages

		//unlock mutexs
		_mqMutex_p->unlock();
	}


	void startLoad(RenderableSetupData *data)
	{
		SDL_Log("Renderer: Entering load state");

		_state = RendererState::loading;

		//just start the load here
		for (auto mdata : data->models)
		{
			if (!mdata.empty())
			{
				ModelLoadingData mld;
				mld.name = mdata;
				_modelLoadQueue_p->push_back(mld);
			}
		}

		for (auto tdata : data->textures)
		{
			if (!tdata.empty())
			{
				TextureLoadingData tld;
				tld.name = tdata;
				_textureLoadQueue_p->push_back(tld);
			}
		}

		//grab context
		acquireContext();

		//TESTONLY set default buffer
		_buffer = RendererBuffer::color;

		//DON'T DO ANYTHING LONG IN HERE BECAUSE THE QUEUE IS STILL BLOCKED
	}

	void startUnload()
	{
		_state = RendererState::unloading;
	}

	/// <summary>
	/// Loads stuff
	/// </summary>
	void doLoad()
	{
		//needs refactoring badly

		//possible future optimization: do things you don't need context for when you don't have context

		//if we don't have context, get context
		if (!haveContext())
			acquireContext();
		if (!haveContext())
			return; //wait for context to be released

		//TODO EVENTUALLY: blit render load screen
		drawLoadScreen();

		//loads stuff

		MessagingSystem *ms = &MessagingSystem::instance();
		if (ms == nullptr)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: messaging system does not exist");
			return;
		}

		//possible future optimization: don't push the entire queues at once
		//dispatch from model and texture load queues
		if (!_modelLoadQueue_p->empty())
		{
			for (int i = 0; i < _modelLoadQueue_p->size(); i++)
			{
				ModelLoadingData mld = _modelLoadQueue_p->at(i);

				FileLoadMessageContent *flmc = new FileLoadMessageContent();
				flmc->path = MODEL_BASEPATH_CONST + mld.name + MODEL_EXTENSION_CONST;
				flmc->relative = true;

				mld.hash = FileEngine::HashFilePath(flmc->path, flmc->relative);

				std::shared_ptr<Message> msg = std::make_shared<Message>(MESSAGE_TYPE::FileLoadMessageType, false);
				msg->setContent(flmc);
				ms->postMessage(msg);

				_modelAwaitQueue_p->push_back(mld);
			}

			_modelLoadQueue_p->clear();
		}

		if (!_textureLoadQueue_p->empty())
		{
			for (int i = 0; i < _textureLoadQueue_p->size(); i++)
			{
				TextureLoadingData tld = _textureLoadQueue_p->at(i);

				FileLoadImageMessageContent *flimc = new FileLoadImageMessageContent();
				flimc->path = TEXTURE_BASEPATH_CONST + tld.name + TEXTURE_EXTENSION_CONST;
				flimc->relative = true;

				tld.hash = FileEngine::HashFilePath(flimc->path, flimc->relative);

				std::shared_ptr<Message> msg = std::make_shared<Message>(MESSAGE_TYPE::FileLoadImageMessageType, false);
				msg->setContent(flimc);
				ms->postMessage(msg);

				_textureAwaitQueue_p->push_back(tld);
			}

			_textureLoadQueue_p->clear();
		}

		//process results from file return queue
		//not optimized for concurrency TODO
		if (!_fmq_p->empty())
		{
			_fmqMutex_p->lock();

			for (int i = 0; i < _fmq_p->size(); i++)
			{
				Message *msg = _fmq_p->at(i).get();
				BaseMessageContent * bmc = msg->getContent();

				switch (msg->getType())
				{
				case MESSAGE_TYPE::FileLoadedMessageType:
					{
						FileLoadedMessageContent *flmc = static_cast<FileLoadedMessageContent*>(bmc);
						findAndLoadModel(flmc);
					}
					break;
				case MESSAGE_TYPE::FileLoadedImageMessageType:
					{
						FileLoadedImageMessageContent *flimc = static_cast<FileLoadedImageMessageContent*>(bmc);
						findAndLoadTexture(flimc);
					}
					break;
				default:
					SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Received unknown message content type ");
					break;
				}
			}

			_fmq_p->clear();

			_fmqMutex_p->unlock();
		}

		//loading is done if and only if both load and await queues are empty and we have context
		if (_textureLoadQueue_p->empty() && _textureAwaitQueue_p->empty() && _modelLoadQueue_p->empty() && _modelAwaitQueue_p->empty() && haveContext())
		{
			std::shared_ptr<Message> msg = std::make_shared<Message>(MESSAGE_TYPE::RenderReadyMessageType, false);
			msg->setContent(new RenderReadyMessageContent());
			ms->postMessage(msg);
			
			SDL_Log("Renderer: Entering render state");
			_state = RendererState::rendering;
		}

	}

	void doSingleLoad()
	{
		//load one thing during drawing process
		//TODO implementation

		//dispatch ONE model and ONE texture from load queues (if nonempty)

		//check the file return queue for ONE file
	}

	void findAndLoadModel(FileLoadedMessageContent *flmc)
	{
		int64_t foundModel = -1;
		ModelLoadingData foundMLD;

		for (int j = 0; j < _modelAwaitQueue_p->size(); j++)
		{
			ModelLoadingData mld = _modelAwaitQueue_p->at(j);
			if (mld.hash == flmc->hash)
			{
				foundMLD = mld;
				foundModel = j;
				break;
			}
		}

		if (foundModel >= 0)
		{
			loadOneModel(foundMLD, &flmc->content);
			_modelAwaitQueue_p->erase(_modelAwaitQueue_p->begin() + foundModel);
		}
	}

	void findAndLoadTexture(FileLoadedImageMessageContent *flimc)
	{
		int64_t foundTexture = -1;
		TextureLoadingData foundTLD;

		for (int j = 0; j < _textureAwaitQueue_p->size(); j++)
		{
			TextureLoadingData tld = _textureAwaitQueue_p->at(j);
			if (tld.hash == flimc->hash)
			{
				foundTLD = tld;
				foundTexture = j;
				break;
			}
		}

		if (foundTexture >= 0)
		{
			loadOneTexture(foundTLD, flimc->image.get());
			_textureAwaitQueue_p->erase(_textureAwaitQueue_p->begin() + foundTexture);
		}
	}

	void loadOneModel(ModelLoadingData mld, std::string *data_p)
	{

		ModelData md;

		//does nothing yet
		auto objData = OBJImport::importObjInfo(*data_p);
		GLuint numVertices = (GLuint)objData.size() / 8;

		GLfloat *objPtr = &objData[0];
		GLuint glVaoId, glVboId;

		glGenVertexArrays(1, &glVaoId);
		glBindVertexArray(glVaoId);
		glGenBuffers(1, &glVboId);
		glBindBuffer(GL_ARRAY_BUFFER, glVboId);
		glBufferData(GL_ARRAY_BUFFER, (objData.size() * sizeof(GLfloat)), objPtr, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0); //vertex coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT))); //normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT))); //UVs
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		md.numVerts = numVertices;
		md.preScale = 1.0f;
		md.vaoID = glVaoId;
		md.vboID = glVboId;

		_models_p->emplace(mld.name, md);

	}

	void loadOneTexture(TextureLoadingData tld, SDL_Surface *image_p)
	{
		TextureData td;

		GLint mode = GL_RGB;

		if (image_p->format->BytesPerPixel == 4)
			mode = GL_RGBA;

		GLuint glTexId;

		glGenTextures(1, &glTexId);
		glBindTexture(GL_TEXTURE_2D, glTexId);

		glTexImage2D(GL_TEXTURE_2D, 0, mode, image_p->w, image_p->h, 0, mode, GL_UNSIGNED_BYTE, image_p->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		td.texID = glTexId;

		_textures_p->emplace(tld.name, td);
		
	}

	/// <summary>
	/// Unloads stuff
	/// </summary>
	void doUnload()
	{
		//TODO EVENTUALLY: blit render load screen
		drawUnloadScreen();

		//delete (some) GL stuff, purge data structures, DO NOT PURGE MESSAGE QUEUES
		unloadGL();
		unloadData();

		//finally release context
		releaseContext();

		_state = RendererState::idle;
	}

	void doIdle()
	{
		//drawIdleScreen();
	}

	void unloadGL()
	{
		//unbind all OGL

		//delete VBOs and VAOs
		for (std::map<std::string, ModelData>::iterator itr = _models_p->begin(); itr != _models_p->end(); itr++)
		{
			ModelData md = itr->second;
			glDeleteVertexArrays(1, &md.vaoID);
			glDeleteBuffers(1, &md.vboID);
		}

		//delete textures
		for (std::map<std::string, TextureData>::iterator itr = _textures_p->begin(); itr != _textures_p->end(); itr++)
		{
			TextureData td = itr->second;
			glDeleteTextures(1, &td.texID);
		}
	}

	void unloadData()
	{
		//clear (but DO NOT DELETE) data structures

		//purge input and file, but NOT main message queue
		_imqMutex_p->lock();
		_imq_p->clear();
		_imqMutex_p->unlock();
		_fmqMutex_p->lock();
		_fmq_p->clear();
		_fmqMutex_p->unlock();

		//purge load queues
		_modelLoadQueue_p->clear();
		_textureLoadQueue_p->clear();

		//purge await queues
		_modelAwaitQueue_p->clear();
		_textureAwaitQueue_p->clear();

		//purge model and texture lists
		_models_p->clear();
		_textures_p->clear();

		//purge current scene and overlay
		if(_lastScene_p != nullptr)
			delete(_lastScene_p);
		_lastScene_p = nullptr;

		if(_lastOverlay_p != nullptr)
			delete(_lastOverlay_p);
		_lastOverlay_p = nullptr;
	}

	void drawLoadScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.1f, 0.75f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void drawUnloadScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.1f, 0.25f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void drawIdleScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.75f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void setupWindow()
	{
		//eventually this may do more

		int width, height;
		if (GlobalPrefs::renderHeight > 0 && GlobalPrefs::renderWidth > 0)
		{
			width = GlobalPrefs::renderWidth;
			height = GlobalPrefs::renderHeight;
		}
		else
		{
			SDL_GL_GetDrawableSize(_window_p, &width, &height);
		}

		_renderWidth = width;
		_renderHeight = height;
	}


	void setupProgram()
	{
		_programID = Shaders::LoadShaders();
		_shaderModelMatrixID = glGetUniformLocation(_programID, "iModelMatrix");
		_shaderMVPMatrixID = glGetUniformLocation(_programID, "iModelViewProjectionMatrix");
		_shaderTextureID = glGetUniformLocation(_programID, "iTexImage");
		_shaderSmoothnessID = glGetUniformLocation(_programID, "iSmoothness");
	}

	void cleanupProgram()
	{
		//delete shaders/program
		if(_programID > 0)
			glDeleteProgram(_programID);
	}

	void setupCube()
	{
		//setup cube (fallback) VAO TODO CHANGE TO LOAD FROM FILE
		glGenVertexArrays(1, &_cubeVertexArrayID);
		glBindVertexArray(_cubeVertexArrayID);

		glGenBuffers(1, &_cubeVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0); //vertex coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT))); //normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT))); //UVs
				
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//setup cube (fallback) texture

		std::string texturePath = TEXTURE_BASEPATH_CONST + "default" + TEXTURE_EXTENSION_CONST;

		SDL_Surface *image_p = FileHelper::loadImageFileFromStringRelative(texturePath);

		GLint mode = GL_RGB;

		if (image_p->format->BytesPerPixel == 4)
			mode = GL_RGBA;

		GLuint glTexId;

		glGenTextures(1, &glTexId);
		glBindTexture(GL_TEXTURE_2D, glTexId);

		glTexImage2D(GL_TEXTURE_2D, 0, mode, image_p->w, image_p->h, 0, mode, GL_UNSIGNED_BYTE, image_p->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(image_p);
		image_p = nullptr;

		_cubeTextureID = glTexId;
	}

	void cleanupCube()
	{
		glDeleteBuffers(1, &_cubeVertexBufferID);
		glDeleteVertexArrays(1, &_cubeVertexArrayID);
		glDeleteTextures(1, &_cubeTextureID);
	}

	void setupFramebuffers()
	{

		//gen FBO
		glGenFramebuffers(1, &_framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

		//gen framebuffer textures
		glGenTextures(1, &_framebufferTexture0ID);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture0ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _renderWidth, _renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _framebufferTexture0ID, 0);

		glGenTextures(1, &_framebufferTexture1ID);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture1ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _renderWidth, _renderHeight, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, _framebufferTexture1ID, 0);

		glGenTextures(1, &_framebufferTexture2ID);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture2ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _renderWidth, _renderHeight, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, _framebufferTexture2ID, 0);

		//gen depthbuffer
		glGenTextures(1, &_framebufferDepthID);
		glBindTexture(GL_TEXTURE_2D, _framebufferDepthID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _renderWidth, _renderHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _framebufferDepthID, 0);

		//configure FBO		
		GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, drawBuffers);

		//quick check
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			SDL_Log("Renderer: Failed to setup framebuffer!");
			throw;
		}

		//unbind all
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void cleanupFramebuffers()
	{
		//delete FBOs
		glDeleteTextures(1, &_framebufferTexture0ID);
		glDeleteTextures(1, &_framebufferTexture1ID);
		glDeleteTextures(1, &_framebufferTexture2ID);
		glDeleteTextures(1, &_framebufferDepthID);
		glDeleteFramebuffers(1, &_framebufferID);
	}

	void setupFramebufferDraw()
	{
		glGenVertexArrays(1, &_framebufferDrawVertexArrayID);
		glBindVertexArray(_framebufferDrawVertexArrayID);

		glGenBuffers(1, &_framebufferDrawVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _framebufferDrawVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_framebufferDrawProgramID = Shaders::LoadShadersFBDraw();

		//get locations
		_framebufferDrawTex0ID = glGetUniformLocation(_framebufferDrawProgramID, "fColor");
		_framebufferDrawTex1ID = glGetUniformLocation(_framebufferDrawProgramID, "fPosition");
		_framebufferDrawTex2ID = glGetUniformLocation(_framebufferDrawProgramID, "fNormal");
		_framebufferDrawTex3ID = glGetUniformLocation(_framebufferDrawProgramID, "fDepth");
		_framebufferDrawBufferID = glGetUniformLocation(_framebufferDrawProgramID, "testBuffer");
	}

	void cleanupFramebufferDraw()
	{
		// delete VBOs
		glDeleteBuffers(1, &_framebufferDrawVertexBufferID);
		glDeleteVertexArrays(1, &_framebufferDrawVertexArrayID);
	}

	/// <summary>
	/// Renders a frame
	/// </summary>
	void doRender()
	{

		//will remain in final
		if (_lastScene_p == nullptr)
		{
			drawNullScene();
		}
		else
		{
			testBuffers(); //check input, swap buffers if need be
			drawCamera(_lastScene_p); //set up the camera
			drawObjects(_lastScene_p); //do the geometry pass
			drawLighting(_lastScene_p); //do the lighting pass
		}

		if (_lastOverlay_p == nullptr)
		{
			drawNullOverlay();
		}
		else
		{
			drawOverlay(_lastOverlay_p);
		}

		//TODO vsync/no vsync
		//SDL_GL_SwapWindow(_window_p);
	}

	void drawNullScene()
	{
		//fallback drawing routine if no scene is available
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GL_GetDrawableSize(_window_p, &w, &h);
		glViewport(0, 0, w, h);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void testBuffers()
	{
		//check input queue, if we have a Y button pressed, change buffer
		bool wasPressed = false;

		_imqMutex_p->lock();

		if (!_imq_p->empty())
		{
			Message *msg = _imq_p->front().get();
			if (msg->getType() == MESSAGE_TYPE::InputMessageType)
			{
				InputMessageContent *imc = static_cast<InputMessageContent*>(msg->getContent());
				if (imc->type == INPUT_TYPES::Y_BUTTON)
					wasPressed = true;
			}
			_imq_p->clear();
		}

		_imqMutex_p->unlock();

		if (wasPressed)
		{
			SDL_Log("Renderer: changing buffer type");

			//set the renderer buffer type
			int pos = static_cast<int>(_buffer);
			pos++;
			if (pos >= static_cast<int>(RendererBuffer::LENGTH))
				pos = 0;
			_buffer = static_cast<RendererBuffer>(pos);
		}	

	}

	void drawCamera(RenderableScene *scene)
	{
		RenderableCamera *camera = &scene->camera;

		//"draw" the camera, actually just set up base matrices
		
		glm::mat4 projection = glm::perspective(camera->viewAngle, (float)_renderWidth / (float)_renderHeight, camera->nearPlane, camera->farPlane);
		glm::mat4 look = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		glm::mat4 translation = glm::translate(look, camera->position * -1.0f);
		glm::mat4 rotation = glm::mat4();
		rotation = glm::rotate(rotation, camera->rotation.z, glm::vec3(0, 0, 1));
		rotation = glm::rotate(rotation, camera->rotation.x, glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, camera->rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 view = rotation * translation;
		_baseModelViewMatrix = view;
		_baseModelViewProjectionMatrix = projection * view;

	}

	void drawObjects(RenderableScene *scene)
	{

		//bind framebuffer and clear
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //TODO use camera color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO draw objects
		for (int i = 0; i < scene->objects.size(); i++)
		{
			RenderableObject *ro = &scene->objects[i];
			drawObject(ro);
		}

	}

	void drawObject(RenderableObject *object)
	{
		//draw one arbitraty object
		//NOTE: should always be tolerant of missing resources!

		//set shader program
		glUseProgram(_programID);

		//check if a model exists
		bool hasModel = false;
		bool hasTexture = false;
		ModelData modelData;
		TextureData texData;

		if (_models_p->count(object->modelName) > 0)
			hasModel = true;
		//else
		//	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Renderer: model is missing!");

		if (_textures_p->count(object->albedoName) > 0)
			hasTexture = true;
		//else
		//	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Renderer: texture is missing!");

		//try to bind model
		if (hasModel)
		{
			modelData = _models_p->find(object->modelName)->second;
			if (modelData.vaoID != 0)
			{
				glBindVertexArray(modelData.vaoID);
			}
			else
			{
				hasModel = false;
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Renderer: model has no VAO!");
			}
				
		}
		
		if(!hasModel)
		{			
			glBindVertexArray(_cubeVertexArrayID);
		}


		//try to bind texture
		if (hasTexture)
		{
			texData = _textures_p->find(object->albedoName)->second;
			if (texData.texID != 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texData.texID);
				glUniform1i(_shaderTextureID, 0);
			}
			else
			{
				hasTexture = false;
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Renderer: texture has no texID!");
			}
		}	

		if (!hasTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _cubeTextureID);
			glUniform1i(_shaderTextureID, 0);
		}

		glUniform1f(_shaderSmoothnessID, object->smoothness);

		//transform!
		glm::mat4 objectMVM = glm::mat4();
		objectMVM = glm::translate(objectMVM, object->position);
		objectMVM = glm::scale(objectMVM, object->scale);
		objectMVM = glm::rotate(objectMVM, object->rotation.y, glm::vec3(0, 1, 0));
		objectMVM = glm::rotate(objectMVM, object->rotation.x, glm::vec3(1, 0, 0));
		objectMVM = glm::rotate(objectMVM, object->rotation.z, glm::vec3(0, 0, 1));
		glm::mat4 objectMVPM = _baseModelViewProjectionMatrix *  objectMVM;
		glm::mat4 objectMVM2 = _baseModelViewMatrix * objectMVM;
		glUniformMatrix4fv(_shaderMVPMatrixID, 1, GL_FALSE, &objectMVPM[0][0]);
		glUniformMatrix4fv(_shaderModelMatrixID, 1, GL_FALSE, &objectMVM[0][0]);

		//draw!
		if (hasModel)
		{
			glDrawArrays(GL_TRIANGLES, 0, modelData.numVerts);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		


		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void drawLighting(RenderableScene *scene)
	{
		//setup framebuffer
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GL_GetDrawableSize(_window_p, &w, &h);
		glViewport(0, 0, w, h);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind shader
		glUseProgram(_framebufferDrawProgramID);	

		//bind buffers
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture0ID);
		glUniform1i(_framebufferDrawTex0ID, 0);
				
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture1ID);
		glUniform1i(_framebufferDrawTex1ID, 1);		
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _framebufferTexture2ID);
		glUniform1i(_framebufferDrawTex2ID, 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _framebufferDepthID);
		glUniform1i(_framebufferDrawTex3ID, 3);

		//"select" buffer (yes it's done in the shader itself, sue me)
		glUniform1i(_framebufferDrawBufferID, static_cast<int>(_buffer));

		//setup vertices
		glBindVertexArray(_framebufferDrawVertexArrayID);

		//draw
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//unbind
		glBindVertexArray(0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void drawOverlay(RenderableOverlay *overlay)
	{
		//TODO draw overlay

		//need to make sure right framebuffer is set, clear depth but not color
	}

	void drawNullOverlay()
	{
		//fallback overlay draw if no overlay is available

	}

	bool acquireContext()
	{
		if (SDL_GL_GetCurrentContext() == _context_p)
		{
			return true; //we already have it!
		}
		else
		{
			int responseCode = SDL_GL_MakeCurrent(_window_p, _context_p);
			if (responseCode == 0)
				return true;
		}

		return false;
	}

	bool releaseContext()
	{
		//does nothing but we may need it later
		return true;
	}

	bool haveContext()
	{
		return SDL_GL_GetCurrentContext() == _context_p;
	}

};

RenderEngine::RenderEngine()
{
	_impl = new RenderEngineImplementation();
}

void RenderEngine::start()
{
	_impl->start();
}

void RenderEngine::update()
{
	_impl->update();
}

RenderEngine::~RenderEngine()
{
	delete(_impl);
};
