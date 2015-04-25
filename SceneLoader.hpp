#pragma once

#include <string>
#include "Scene.hpp"

class SceneLoader
{
private:

public:

    static Scene* loadScene(const std::string& filename);
};

