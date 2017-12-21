#include "Engine.h"
Engine::Engine()
{
	this->Setup();
};
Engine::~Engine()
{

};
void Engine::Setup()
{

};
void Engine::Start()
{
	SetEngineRunning(true);
	this->Loop();
};
void Engine::Stop()
{
	SetEngineRunning(false);
};
void Engine::Loop()
{
	while (this->engineRunning)
	{
		UpdateObjects();
	}
};
void Engine::SetEngineRunning(bool action)
{
	this->engineRunning = action;
};
bool Engine::GetEngineRunning()
{
	return this->engineRunning;
};
void Engine::UpdateObjects()
{

};