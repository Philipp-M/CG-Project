#include <cstddef>
#include <glm/ext.hpp>
#include "Shader.hpp"
#include "picojson.hpp"
#include "Scene.hpp"
#include "ShaderProgramManager.hpp"

Scene::Scene(const std::string &filename)
{
    GLuint shaderId = ShaderProgramManager::get().addShaderProgram(
            new ShaderProgram("default", Shader("vertex", "vertexshader.vs", VERTEX),
                              Shader("fragment", "fragmentshader.fs", FRAGMENT)));
    shaderProgram = ShaderProgramManager::get().getShaderById(shaderId);
    loadFromFile(filename);
}

Scene::~Scene()
{
    for (std::vector<Model*>::iterator it=models.begin(); it!=models.end(); ++it)
        delete (*it);
}
bool Scene::loadFromFile(const std::string &filename)
{
    // TODO with json as scene file
}

/**
 * just a temporary method until the scene loader is finished(see above)
 */
void Scene::addModel(Model *model)
{
    if (model != NULL)
        models.push_back(model);
}

void Scene::draw()
{
    shaderProgram->bind();
    shaderProgram->setMatrixUniform4f("ViewMatrix", camera.getTransformationMatrix());
    for (std::vector<Model*>::iterator it=models.begin(); it!=models.end(); ++it)
    {
        if((*it)!= NULL)
            (*it)->draw(shaderProgram);
    }
    shaderProgram->unbind();
}

Camera &Scene::getCamera()
{
    return camera;
}

Model *Scene::getModel(const std::string &name)
{
    for (std::vector<Model*>::iterator it=models.begin(); it!=models.end(); ++it)
    {
        if (*it != NULL && (*it)->getName() == name)
            return *it;
    }
    return NULL;
}

