#include <cmath>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

GlutMainLoop::GlutMainLoop() : movementSpeed(0.005), merrySpeed(-0.0005), maxMerryHeight(0.3), merryHeight1(0), automaticMode(true),
                               useAmbientLightning(true), useDiffuseLightning(true), useSpecularLightning(true)
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
	std::cout << "FPS: " << 1000.0/deltaElapsedTime << std::endl;
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
	std::vector<PointLight> &pLights = scene->getPointLights();
	for (int i = 0; i < pLights.size(); i++)
	{
		if (i == 3)
			pLights[3].setPosition(
				glm::vec3(10 * sin(0.0005 * glutGet(GLUT_ELAPSED_TIME)), 10 * cos(0.0007 * glutGet(GLUT_ELAPSED_TIME)), -10));
	}
	float oldMerryHeight1 = merryHeight1;
	float oldMerryHeight2 = merryHeight2;
	float oldMerryRotation = merryRotation;
	for (int i = 0; i < models.size(); i++)
	{
		const std::string &name = models[i]->getName();
		// following if-statement is *not* super efficent, but thats not the subject of the course... just wanna note that...
		if (name == "horse1" || name == "horse2" || name == "horse3" || name == "horse4" || name == "horse5" || name == "horse6" ||
		    name == "horse7" || name == "horse8" || name == "Top" || name == "Bottom" || name == "Pole" || name == "HandPoles" ||
		    name == "healthBar" || name == "LightMerry1" || name == "LightMerry2" || name == "LightMerry3" || name == "LightMerry4" ||
		    name == "LightMerry5")
		{
			merryRotation = merrySpeed * deltaElapsedTime;
			models[i]->rotate(glm::vec3(0.000, 0.000, merryRotation));
		}
		if (models[i]->getName() == "horse1" || models[i]->getName() == "horse3" || models[i]->getName() == "horse5" ||
		    models[i]->getName() == "horse7" || name == "healthBar")
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
	// set the position to the green horse
	scene->getCameraSystem().setPosition(glm::vec3(0, 3.514, 1.623));
	scene->getCameraSystem().setRotation(glm::vec3(M_PI / 2, 0, M_PI / 2));

//	scene->addPointLight(PointLight(glm::vec3(10, 10, -10), Color::fromRGB(glm::vec3(150*1.0, 150*0.8, 150*0.6)), 1.0, 0.3));
//	scene->addPointLight(PointLight(glm::vec3(100, 100, -100), Color::fromRGB(glm::vec3(1.0, 0.8, 0.6)));
//	scene->addPointLight(PointLight(glm::vec3(-10, -10, -10), Color::fromRGB(glm::vec3(0.0, 1.0, 0.0))));
//	scene->addPointLight(PointLight(glm::vec3(-10, 15, -10), Color::fromRGB(glm::vec3(1.0, 0.0, 0.0)), 170));
//	scene->addPointLight(PointLight(glm::vec3(10, -10, -20), Color::fromRGB(glm::vec3(0.0, 0.0, 1.0)), 150));
//	scene->addPointLight(PointLight(glm::vec3(1.5, -1.5, -1), Color::fromRGB(glm::vec3(0.0, 1.0, 1.0)), 5, 0.2));
//	scene->addPointLight(PointLight(glm::vec3(1.5, 1.5, -1), Color::fromRGB(glm::vec3(1.0, 1.0, 1.0)), 4, 0.3));
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
		case 'y':
			useDiffuseLightning = !useDiffuseLightning;
	        break;
		case 'x':
			useSpecularLightning = !useSpecularLightning;
	        break;
		case 'c':
			useAmbientLightning = !useAmbientLightning;
	        break;
		case 'v':
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i]->isLight())
				{
					Color &c = models[i]->getMaterial()->difColor;
					c.setHue(360.0f * (rand() % INT32_MAX) / (float) INT32_MAX);
				}
			}
	        break;
		case 'b':
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i]->isLight())
				{
					Color &c = models[i]->getMaterial()->difColor;
					c.setValue(c.getHSV().z * 1.1);
				}
			}
	        break;
		case 'n':
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i]->isLight())
				{
					Color &c = models[i]->getMaterial()->difColor;
					c.setValue(c.getHSV().z * 0.9);
				}
			}
	        break;
		case 'm':
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i]->isLight())
				{
					Color &c = models[i]->getMaterial()->difColor;
					c.setSaturation(c.getHSV().y * 1.1);
				}
			}
	        break;
		case ',':
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i]->isLight())
				{
					Color &c = models[i]->getMaterial()->difColor;
					c.setSaturation(c.getHSV().y * 0.9);
				}
			}
	        break;
		case 't':
			automaticMode = !automaticMode;
	        merryRotation = 0;
	        merryHeight1 = 0;
	        merryHeight2 = 0;
	        scene->getCameraSystem().setPosition(glm::vec3(0, 3.514, 1.623));
	        scene->getCameraSystem().setRotation(glm::vec3(M_PI / 2, 0, M_PI / 2));
	        for (int i = 0; i < models.size(); i++)
		        models[i]->resetTransformationMatrix();
	        break;

		default:
			break;
	}
}

bool GlutMainLoop::isUseDiffuseLightning() const
{
	return useDiffuseLightning;
}

bool GlutMainLoop::isUseSpecularLightning() const
{
	return useSpecularLightning;
}

bool GlutMainLoop::isUseAmbientLightning() const
{
	return useAmbientLightning;
}