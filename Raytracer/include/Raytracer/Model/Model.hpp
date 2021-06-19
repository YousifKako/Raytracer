#pragma once

#include <vector>

#include <Model/Mesh.hpp>

class Model
{
private:
    std::string model_name = "";
    std::vector<Mesh*>* model = new std::vector<Mesh*>();
    std::uint32_t num_objs = 0;

public:
    Model(const std::string_view& model_name);

    void load();
    const std::vector<Mesh*>* const get();
};