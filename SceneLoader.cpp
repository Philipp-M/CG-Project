#include <fstream>
#include <cmath>
#include "SceneLoader.hpp"
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
    VertexBuilder(const std::vector<GLuint> &indices, const tinyobj::mesh_t &mesh,
                  const std::vector<tinyobj::material_t> &materials) : indices(indices), mesh(mesh), materials(materials),
                                                                       tempAdjVert(indices.size() / 3)
    {
    }

    void computeVertices(std::vector<Model::Vertex3> &vertices)
    {
        calculateFaceNormals(vertices);
        vertices.resize(mesh.positions.size());
        for (GLuint i = 0; i < mesh.positions.size() / 3; i++)
        {
            vertices[i] = Model::Vertex3(glm::vec3(mesh.positions[i], mesh.positions[i + 1], mesh.positions[i + 2]), glm::vec3(),
                                         glm::vec3());
            calculateNormalColor(vertices[i], i);
        }
    }
};

Scene *SceneLoader::loadScene(const std::string &filename)
{
    bool foundObjFilename = false;
    bool foundMtlFilename = false;
    bool foundCameraWidth = false;
    bool foundCameraHeight = false;
    bool foundCameraFOV = false;
    bool foundCameraPosition = false;
    bool foundCameraRotation = false;
    int cameraWidth;
    int cameraHeight;
    double cameraFOV;
    glm::vec3 cameraPosition;
    glm::vec3 cameraRotation;
    std::string objFilename;
    std::string mtlFilename;
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
        else if (i->first == "MtlFilename")
        {
            if (i->second.is<std::string>())
            {
                foundMtlFilename = true;
                mtlFilename = i->second.get<std::string>();
            }
            else
            {
                std::cerr << "MtlFilename is not a string" << std::endl;
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
                                else if(!k->is<double>())
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
                                else if(!k->is<double>())
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
    if (!(foundObjFilename && foundMtlFilename && foundCameraWidth && foundCameraHeight && foundCameraFOV &&
          foundCameraPosition && foundCameraRotation))
    {
        std::cerr << "some attributes are missing in the scene file" << std::endl;
        exit(3);
    }

    return NULL;
}

void SceneLoader::parseScene()
{
    picojson::value v;
}
