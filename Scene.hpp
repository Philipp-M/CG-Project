#pragma once

#include <string>
#include <vector>
#include "Camera.hpp"
#include "Model.hpp"

class Scene
{
private:
    Camera camera;
    std::vector<Model *> models;
    /**
     * temprorary variable will hopefully in future be loaded with in the scene loader or
     * with functions that recognize which material they are inside the obj file
     */
    const ShaderProgram *shaderProgram;
public:
    Scene(const std::string &filename);
    ~Scene();
    void draw();

    void addModel(Model *model);

    const std::vector<Model *>& getModels();

    bool loadFromFile(const std::string &filename);

    Camera & getCamera();
    Model * getModel(const std::string& name);
    static Scene* loadFromObj(const std::string& filename);
};
