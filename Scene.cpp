#include <cstddef>
#include <glm/ext.hpp>
#include "Shader.hpp"
#include "picojson.hpp"
#include "Scene.hpp"
#include "ShaderProgramManager.hpp"
#include "tiny_obj_loader.hpp"

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
    for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
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
    for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
    {
        if ((*it) != NULL)
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
    for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
    {
        if (*it != NULL && (*it)->getName() == name)
            return *it;
    }
    return NULL;
}

Scene *Scene::loadFromObj(const std::string &filename)
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), NULL);

    if (!err.empty())
    {
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }
    Scene *retScene = new Scene("");
    for (size_t i = 0; i < shapes.size(); i++)
    {
        const std::string &name = shapes[i].name;
        const std::vector<GLuint> &iData = shapes[i].mesh.indices;
        std::vector<Model::Vertex3> vData;
        for (size_t f = 0; f < shapes[i].mesh.positions.size() / 3; f++)
        {
            // color is not correct, there needs to be done more, since it's bound to the indices instead of the vertices...
            vData.push_back(Model::Vertex3(glm::vec3(shapes[i].mesh.positions[f * 3 + 0],
                                                     shapes[i].mesh.positions[f * 3 + 1],
                                                     shapes[i].mesh.positions[f * 3 + 2]),
                                           glm::vec3(materials[shapes[i].mesh.material_ids[0]].diffuse[0],
                                                     materials[shapes[i].mesh.material_ids[0]].diffuse[1],
                                                     materials[shapes[i].mesh.material_ids[0]].diffuse[2])));
        }
        std::cout << "adding model: " << name << " with " << shapes[i].mesh.positions.size() << " vertices " << std::endl;
        retScene->addModel(new Model(name, vData, iData));
    }
    return retScene;
}

const std::vector<Model *> &Scene::getModels()
{
    return models;
}
