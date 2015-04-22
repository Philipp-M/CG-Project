#include <glm/ext.hpp>
#include "picojson.hpp"
#include "Scene.hpp"
#include "ShaderProgramManager.hpp"

Scene::Scene(const std::string &filename)
{
    GLuint shaderId = ShaderProgramManager::get().addShaderProgram(
            new ShaderProgram("default", Shader("vertex", "vertexshader.vs", Shader::ShaderType::VERTEX),
                              Shader("fragment", "fragmentshader.fs", Shader::ShaderType::FRAGMENT)));
    shaderProgram = ShaderProgramManager::get().getShaderById(shaderId);
    loadFromFile(filename);
}

Scene::~Scene()
{
    for(auto model : models)
        delete model;
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
    if (model != nullptr)
        models.push_back(model);
}

void Scene::draw()
{
    shaderProgram->bind();
    shaderProgram->setMatrixUniform4f("ViewMatrix", camera.getTransformationMatrix());
    for (auto model : models)
    {
        if(model != nullptr)
            model->draw(shaderProgram);
    }
    shaderProgram->unbind();
}

Camera &Scene::getCamera()
{
    return camera;
}

Model *Scene::getModel(const std::string &name)
{
    for (auto model : models)
    {
        if (model != nullptr && model->getName() == name)
            return model;
    }
    return nullptr;
}

