#include <cmath>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

GlutMainLoop::GlutMainLoop()
{
}

void GlutMainLoop::onDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->draw();
	glutSwapBuffers();
}

void GlutMainLoop::onIdle()
{
	uint32_t deltaElapsedTime = glutGet(GLUT_ELAPSED_TIME) - lastElapsedtime;
	CameraSystem &cameraSystem = scene->getCameraSystem();
	cameraSystem.getCamera().setWidth(glutGet(GLUT_WINDOW_WIDTH));
	cameraSystem.getCamera().setHeight(glutGet(GLUT_WINDOW_HEIGHT));
	// move the camera
	if(pressedKeys['w'])
		cameraSystem.moveForward(deltaElapsedTime*0.005);
	if(pressedKeys['s'])
		cameraSystem.moveBackward(deltaElapsedTime*0.005);
	if(pressedKeys['a'])
		cameraSystem.moveLeft(deltaElapsedTime*0.005);
	if(pressedKeys['d'])
		cameraSystem.moveRight(deltaElapsedTime*0.005);
	const std::vector<Model *> &models = scene->getModels();
	for (int i = 0; i < models.size(); i++)
	{
		const std::string &name = models[i]->getName();
		// following if-statement is *not* super efficent, but thats not the subject of the course... just wanna note that...
		if (name == "horse1" || name == "horse2" || name == "horse3" || name == "horse4" || name == "horse5" || name == "horse6" ||
		    name == "horse7" || name == "horse8" || name == "Top" || name == "Bottom" || name == "Pole" || name == "HandPoles")
		{
			models[i]->resetTransformationMatrix();
			models[i]->rotate(glm::vec3(0.000, 0.000, -0.0005 * glutGet(GLUT_ELAPSED_TIME)));
		}
		if (models[i]->getName() == "horse1" || models[i]->getName() == "horse3" || models[i]->getName() == "horse5" ||
		    models[i]->getName() == "horse7")
		{
			models[i]->move(glm::vec3(0, 0, -0.3 + 0.3 * std::sin(0.0015 * glutGet(GLUT_ELAPSED_TIME))));
		}
		if (models[i]->getName() == "horse2" || models[i]->getName() == "horse4" || models[i]->getName() == "horse6" ||
		    models[i]->getName() == "horse8")
		{
			models[i]->move(glm::vec3(0, 0, -0.3 + 0.3 * std::sin(0.0025 * glutGet(GLUT_ELAPSED_TIME))));
		}

	}
	lastElapsedtime = glutGet(GLUT_ELAPSED_TIME);
	glutPostRedisplay();
}

//ugly hack necessary due to dumb c++
void GlutMainLoop::mouseFuncStatic(int x, int y)
{
	GlutMainLoop::get().mouseFunc(x, y);
}

//ugly hack necessary due to dumb c++
void GlutMainLoop::keyboardFuncStatic(uint8_t k, int x, int y)
{
	GlutMainLoop::get().keyboardFunc(k, x, y);
}

void GlutMainLoop::keyboardUpFuncStatic(uint8_t k, int x, int y)
{
	GlutMainLoop::get().keyboardUpFunc(k, x, y);
}

void GlutMainLoop::init()
{
	// register all the eventlisteners
	glutPassiveMotionFunc(GlutMainLoop::mouseFuncStatic);
	glutMotionFunc(GlutMainLoop::mouseFuncStatic);
	glutKeyboardFunc(GlutMainLoop::keyboardFuncStatic);
	glutKeyboardUpFunc(GlutMainLoop::keyboardUpFuncStatic);
	glutIgnoreKeyRepeat(1);
	// disable the cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	// load the scene
	scene = new Scene("./scene/merry.json");
}

GlutMainLoop::~GlutMainLoop()
{
	delete scene;
}

void GlutMainLoop::mouseFunc(int x, int y)
{
	int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	scene->getCameraSystem().rotate(glm::vec3(0.002 * (y - centerY), 0, -0.002 * (x - centerX)));
	if (x != centerX || y != centerY)
		glutWarpPointer(centerX, centerY);
}

void GlutMainLoop::keyboardUpFunc(uint8_t k, int x, int y)
{
	pressedKeys[k] = false;
}

void GlutMainLoop::keyboardFunc(uint8_t k, int x, int y)
{
	pressedKeys[k] = true;
}
