#pragma once

//forward declaration of class makes PIMPL possible
class RenderSystemImplementation;

class RenderSystem {
public:
	/// <summary>
	/// Starts the RenderSystem, creates the rendering thread, and switches OpenGL context
	/// </summary>
    void start();

	/// <summary>
	/// Run by engine on every tick, synchronized update on engine thread
	/// </summary>
    void update();

    RenderSystem();
    ~RenderSystem();
private:
	RenderSystemImplementation *_impl;
	
};