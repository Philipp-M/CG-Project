#include "SceneLoader.hpp"
#include "tiny_obj_loader.hpp"

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
        if(tempAdjVertCount != 0)
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
            calculateNormalColor(vertices[i],i);
        }
    }
};
