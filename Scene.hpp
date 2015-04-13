#pragma once

#include <string>
#include <vector>

class Scene {
private:
    // Camera camera TODO
    // std::vector<Model> models; TODO
public:
    Scene(const std::string& filename);
    bool loadFromFile(const std::string& filename);

};

