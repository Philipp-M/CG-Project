#include <cmath>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

GlutMainLoop::GlutMainLoop() : movementSpeed(0.005), merrySpeed(-0.0005), maxMerryHeight(0.3), merryHeight1(0), automaticMode(false)
{
	for (int i = 0; i < 256; i++)
		pressedKeys[i] = false;
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
	if (!automaticMode)
	{
		if (pressedKeys['w'])
			cameraSystem.moveForward(deltaElapsedTime * movementSpeed);
		if (pressedKeys['s'])
			cameraSystem.moveBackward(deltaElapsedTime * movementSpeed);
		if (pressedKeys['a'])
			cameraSystem.moveLeft(deltaElapsedTime * movementSpeed);
		if (pressedKeys['d'])
			cameraSystem.moveRight(deltaElapsedTime * movementSpeed);
	}
	const std::vector<Model *> &models = scene->getModels();
	float oldMerryHeight1 = merryHeight1;
	float oldMerryHeight2 = merryHeight2;
	float oldMerryRotation = merryRotation;
	for (int i = 0; i < models.size(); i++)
	{
		const std::string &name = models[i]->getName();
		// following if-statement is *not* super efficent, but thats not the subject of the course... just wanna note that...
		if (name == "horse1" || name == "horse2" || name == "horse3" || name == "horse4" || name == "horse5" || name == "horse6" ||
		    name == "horse7" || name == "horse8" || name == "Top" || name == "Bottom" || name == "Pole" || name == "HandPoles")
		{
			merryRotation = merrySpeed * deltaElapsedTime;
			models[i]->rotate(glm::vec3(0.000, 0.000, merryRotation));
		}
		if (models[i]->getName() == "horse1" || models[i]->getName() == "horse3" || models[i]->getName() == "horse5" ||
		    models[i]->getName() == "horse7")
		{
			merryHeight1 = (float) (-maxMerryHeight + maxMerryHeight * std::sin(0.0015 * glutGet(GLUT_ELAPSED_TIME)));
			models[i]->move(glm::vec3(0, 0, merryHeight1 - oldMerryHeight1));
		}
		if (models[i]->getName() == "horse2" || models[i]->getName() == "horse4" || models[i]->getName() == "horse6" ||
		    models[i]->getName() == "horse8")
		{
			merryHeight2 = (float) (-maxMerryHeight + maxMerryHeight * std::sin(0.0025 * glutGet(GLUT_ELAPSED_TIME)));
			models[i]->move(glm::vec3(0, 0, merryHeight2 - oldMerryHeight2));
		}

	}
	if (automaticMode)
	{
		const glm::vec3 &p = scene->getCameraSystem().getPosition();
		scene->getCameraSystem().setPosition(glm::vec3(p.x, p.y, 1.62 - merryHeight1));
		cameraSystem.rotateAroundAxis(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), merryRotation);
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
	scene->addPointLight(PointLight(glm::vec3(10, 10, -10), glm::vec3(1.0, 0.8, 0.6), 150));
	scene->addPointLight(PointLight(glm::vec3(-10, -10, -10), glm::vec3(0.0, 1.0, 0.0), 20));
	scene->addPointLight(PointLight(glm::vec3(-10, 15, -10), glm::vec3(1.0, 0.0, 0.0), 70));
	scene->addPointLight(PointLight(glm::vec3(10, -10, -20), glm::vec3(0.0, 0.0, 1.0), 150));
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
	const std::vector<Model *> &models = scene->getModels();
	switch (k)
	{
		case '+':
			movementSpeed *= 1.2;
	        break;
		case '-':
			movementSpeed *= 0.8;
	        break;
		case '*':
			merrySpeed -= 0.0001;
	        break;
		case '_':
			merrySpeed += 0.0001;
	        break;
		case 't':
			automaticMode = !automaticMode;
	        for (int i = 0; i < models.size(); i++)
		        models[i]->resetTransformationMatrix();
	        merryRotation = 0;
	        merryHeight1 = 0;
	        merryHeight2 = 0;
	        scene->getCameraSystem().setPosition(glm::vec3(0, 3.514, 1.623));
	        scene->getCameraSystem().setRotation(glm::vec3(M_PI / 2, 0, M_PI / 2));
	        break;

		default:
			break;
	}
}
