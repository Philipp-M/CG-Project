#pragma once


#include "Scene.hpp"

class GlutMainLoop
{
private:
	Scene *scene;
	bool pressedKeys[256];
	int lastElapsedtime;
	float movementSpeed;
	float merrySpeed;
	float merryRotation;
	float maxMerryHeight;
	float merryHeight1;
	float merryHeight2;
	bool automaticMode;

	GlutMainLoop();

	void mouseFunc(int x, int y);

	static void mouseFuncStatic(int x, int y);

	static void keyboardFuncStatic(uint8_t k, int x, int y);

	static void keyboardUpFuncStatic(uint8_t k, int x, int y);

public:
	~GlutMainLoop();

	static GlutMainLoop &get();

	void init();

	void onIdle();

	void onDraw();

	void keyboardFunc(uint8_t k, int x, int y);

	void keyboardUpFunc(uint8_t k, int x, int y);

};


inline GlutMainLoop &GlutMainLoop::get()
{
	static GlutMainLoop instance;
	return instance;
}

