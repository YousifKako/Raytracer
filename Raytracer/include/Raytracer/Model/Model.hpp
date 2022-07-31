#pragma once

#include <vector>

#include <Model/Mesh.hpp>

class Model
{
private:
    std::vector<Mesh> model = { };
    std::uint32_t num_objs  = 0;

public:
    Model(const std::string& model_name);

    const std::vector<std::string> split(std::string const& original, char separator);
    const std::vector<Mesh>& get();
};