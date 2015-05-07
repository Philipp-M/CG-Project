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
	Camera &camera = scene->getCamera();
	camera.setWidth(glutGet(GLUT_WINDOW_WIDTH));
	camera.setHeight(glutGet(GLUT_WINDOW_HEIGHT));
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
	glutPostRedisplay();
}

void GlutMainLoop::init()
{
	scene = new Scene("./scene/merry.json");
}

GlutMainLoop::~GlutMainLoop()
{
	delete scene;
}
