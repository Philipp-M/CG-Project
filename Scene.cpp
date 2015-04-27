#include <cstddef>
#include <cmath>
#include <iostream>
#include <fstream>
#include <glm/ext.hpp>
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShaderProgramManager.hpp"
#include "tiny_obj_loader.hpp"
#include "picojson.hpp"

class VertexBuilder
{
private:
    std::vector<GLuint> tempAdjVert;
    std::vector<glm::vec3> faceNormals;
    const std::vector<GLuint> &indices;
    const std::vector<tinyobj::material_t> &materials;
    const tinyobj::mesh_t &mesh;

    glm::vec3 calculateFaceNormal(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
    {
        return glm::normalize(glm::cross(v3 - v2, v1 - v2));
    }

    void calculateFaceNormals(const std::vector<Model::Vertex3> &vertices)
    {
        faceNormals.reserve(indices.size() / 3);
        for (GLuint f = 0; f < indices.size() / 3; f++)
            faceNormals.push_back(calculateFaceNormal(vertices[indices[f * 3]].vertex, vertices[indices[f * 3 + 1]].vertex,
                                                      vertices[indices[f * 3 + 2]].vertex));
    }

    void calculateNormalColor(Model::Vertex3 &vertex, GLuint id)
    {
        GLuint tempAdjVertCount = 0;
        for (GLuint f = 0; f < indices.size() / 3; f++)
        {
            if (indices[f * 3 + 0] == id)
                tempAdjVert[tempAdjVertCount++] = f;
            else if (indices[f * 3 + 1] == id)
                tempAdjVert[tempAdjVertCount++] = f;
            else if (indices[f * 3 + 2] == id)
                tempAdjVert[tempAdjVertCount++] = f;
        }
        glm::vec3 vn(0);
        glm::vec3 vc(0);
        for (int i = 0; i < tempAdjVertCount; i++)
        {
            GLuint f = tempAdjVert[i];
            vn += faceNormals[f];
            vc.r += materials[mesh.material_ids[f]].diffuse[0];
            vc.g += materials[mesh.material_ids[f]].diffuse[1];
            vc.b += materials[mesh.material_ids[f]].diffuse[2];
            // maybe add specular color for future...
        }
        // should not be zero, just for safety
        if (tempAdjVertCount != 0)
            vc /= tempAdjVertCount;
        vertex.color = vc;
        vertex.normal = glm::normalize(vn);
    }

public:
    VertexBuilder(const tinyobj::mesh_t &mesh,
                  const std::vector<tinyobj::material_t> &materials) : indices(mesh.indices), mesh(mesh), materials(materials),
                                                                       tempAdjVert(mesh.indices.size() / 3)
    {
    }

    void computeVertices(std::vector<Model::Vertex3> &vertices)
    {
        calculateFaceNormals(vertices);
        vertices.resize(mesh.positions.size());
        for (GLuint i = 0; i < mesh.positions.size() / 3; i++)
        {
            vertices[i] = Model::Vertex3(glm::vec3(mesh.positions[i*3], mesh.positions[i*3 + 1], mesh.positions[i*3 + 2]), glm::vec3(),
                                         glm::vec3());
            calculateNormalColor(vertices[i], i);
        }
    }
};


Scene::Scene(const std::string &filename)
{
    // create a new shader program based on the given file names "vertexshader.vs" and "fragmentshader.fs"
    // add the shader to the shaderProgramManager singleton, for maybe later independent use
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
    bool foundObjFilename = false;
    bool foundMtlBasepath = false;
    bool foundCameraWidth = false;
    bool foundCameraHeight = false;
    bool foundCameraNearPlane = false;
    bool foundCameraFarPlane = false;
    bool foundCameraFOV = false;
    bool foundCameraPosition = false;
    bool foundCameraRotation = false;
    int cameraWidth;
    int cameraHeight;
    double cameraNearPlane;
    double cameraFarPlane;
    double cameraFOV;
    glm::vec3 cameraPosition;
    glm::vec3 cameraRotation;
    std::string objFilename;
    std::string mtlBasepath;
    std::ifstream file(filename.c_str());
    picojson::value v;
    file >> v;

    std::string err = picojson::get_last_error();
    if (!err.empty())
    {
        std::cerr << err << std::endl;
        exit(1);
    }

    // check if the type of the value is "object"
    if (!v.is<picojson::object>())
    {
        std::cerr << "JSON is not an object" << std::endl;
        exit(2);
    }
    const picojson::value::object &obj = v.get<picojson::object>();
    for (picojson::value::object::const_iterator i = obj.begin();
         i != obj.end();
         ++i)
    {
        if (i->first == "ObjFilename")
        {
            if (i->second.is<std::string>())
            {
                foundObjFilename = true;
                objFilename = i->second.get<std::string>();
            }
            else
            {
                std::cerr << "ObjFilename is not a string" << std::endl;
                exit(3);
            }
        }
        else if (i->first == "MtlBasepath")
        {
            if (i->second.is<std::string>())
            {
                foundMtlBasepath = true;
                mtlBasepath = i->second.get<std::string>();
            }
            else
            {
                std::cerr << "MtlBasepath is not a string" << std::endl;
                exit(4);
            }
        }
        else if (i->first == "Camera")
        {
            if (i->second.is<picojson::object>())
            {
                const picojson::value::object &camobj = i->second.get<picojson::object>();
                for (picojson::value::object::const_iterator j = camobj.begin(); j != camobj.end(); ++j)
                {
                    if (j->first == "Width")
                    {
                        if (j->second.is<double>())
                        {
                            foundCameraWidth = true;
                            cameraWidth = (int) j->second.get<double>();
                        }
                        else
                        {
                            std::cerr << "Camera.Width is not a number" << std::endl;
                            exit(5);
                        }
                    }
                    if (j->first == "Height")
                    {
                        if (j->second.is<double>())
                        {
                            foundCameraHeight = true;
                            cameraHeight = (int) j->second.get<double>();
                        }
                        else
                        {
                            std::cerr << "Camera.Height is not a number" << std::endl;
                            exit(6);
                        }
                    }
                    if (j->first == "NearPlane")
                    {
                        if (j->second.is<double>())
                        {
                            foundCameraNearPlane = true;
                            cameraNearPlane = (int) j->second.get<double>();
                        }
                        else
                        {
                            std::cerr << "Camera.NearPlane is not a number" << std::endl;
                            exit(6);
                        }
                    }
                    if (j->first == "FarPlane")
                    {
                        if (j->second.is<double>())
                        {
                            foundCameraFarPlane = true;
                            cameraFarPlane = (int) j->second.get<double>();
                        }
                        else
                        {
                            std::cerr << "Camera.FarPlane is not a number" << std::endl;
                            exit(6);
                        }
                    }
                    if (j->first == "FOV")
                    {
                        if (j->second.is<double>())
                        {
                            foundCameraFOV = true;
                            cameraFOV = j->second.get<double>();
                        }
                        else
                        {
                            std::cerr << "Camera.FOV is not a number" << std::endl;
                            exit(7);
                        }
                    }
                    if (j->first == "Position")
                    {
                        if (j->second.is<picojson::array>())
                        {
                            foundCameraPosition = true;
                            const picojson::array &a = j->second.get<picojson::array>();
                            int c = 0;
                            for (picojson::array::const_iterator k = a.begin(); k != a.end(); ++k)
                            {
                                if (k->is<double>() && c < 3)
                                    cameraPosition[c] = k->get<double>();
                                else if (!k->is<double>())
                                {
                                    std::cerr << "Camera.Position is not an array of numbers" << std::endl;
                                    exit(8);
                                }
                                c++;
                            }
                            if (c != 3)
                            {
                                std::cerr << "Camera.Position is not an array of length 3" << std::endl;
                                exit(9);
                            }
                        }
                        else
                        {
                            std::cerr << "Camera.Position is not an array" << std::endl;
                            exit(10);
                        }
                    }
                    if (j->first == "Rotation")
                    {
                        if (j->second.is<picojson::array>())
                        {
                            foundCameraRotation = true;
                            const picojson::array &a = j->second.get<picojson::array>();
                            int c = 0;
                            for (picojson::array::const_iterator k = a.begin(); k != a.end(); ++k)
                            {
                                if (k->is<double>() && c < 3)
                                    cameraRotation[c] = k->get<double>() * M_PI / 180.0;
                                else if (!k->is<double>())
                                {
                                    std::cerr << "Camera.Rotation is not an array of numbers" << std::endl;
                                    exit(8);
                                }
                                c++;
                            }
                            if (c != 3)
                            {
                                std::cerr << "Camera.Rotation is not an array of length 3" << std::endl;
                                exit(9);
                            }
                        }
                        else
                        {
                            std::cerr << "Camera.Rotation is not an array" << std::endl;
                            exit(10);
                        }
                    }
                }
            }
            else
            {
                std::cerr << "Camera is not a json-Object" << std::endl;
                exit(5);
            }
        }
    }
    if (!(foundObjFilename && foundMtlBasepath && foundCameraWidth && foundCameraHeight && foundCameraNearPlane && foundCameraFarPlane &&
          foundCameraFOV && foundCameraPosition && foundCameraRotation))
    {
        std::cerr << "some attributes are missing in the scene file" << std::endl;
        exit(3);
    }
    /************** finished parsing time for the creation of the scene **************/

    /************** loading all the models **************/
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    err = tinyobj::LoadObj(shapes, materials, objFilename.c_str(), NULL);

    if (!err.empty())
    {
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }
    deleteAllModels();
    for (size_t i = 0; i < shapes.size(); i++)
    {
        const std::string &name = shapes[i].name;
        const std::vector<GLuint> &iData = shapes[i].mesh.indices;
        std::vector<Model::Vertex3> vData(shapes[i].mesh.positions.size()/3);
        VertexBuilder vb(shapes[i].mesh, materials);
        vb.computeVertices(vData);
        std::cout << "adding model: " << name << " with " << shapes[i].mesh.positions.size() << " vertices " << std::endl;
        addModel(new Model(name, vData, iData));
    }
    /*********** setup Camera ***********/
    camera = Camera(cameraWidth, cameraHeight, cameraNearPlane, cameraFarPlane, cameraFOV);
    camera.move(cameraPosition);
    camera.rotate(cameraRotation);
}

void Scene::deleteAllModels()
{
    for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
        delete (*it);
    models = std::vector<Model *>();
}

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
                                                     shapes[i].mesh.positions[f * 3 + 2]), glm::vec3(0),
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

