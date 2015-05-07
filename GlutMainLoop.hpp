#pragma once


#include "Scene.hpp"

class GlutMainLoop
{
private:
	Scene *scene;

	GlutMainLoop();

public:
	~GlutMainLoop();

	static GlutMainLoop &get();

	void init();

	void onIdle();

	void onDraw();
};


inline GlutMainLoop &GlutMainLoop::get()
{
	static GlutMainLoop instance;
	return instance;
}

