#pragma once

#include <vector>

#include <Objects/Triangle.hpp>

class Mesh
{
public:
    std::vector<Triangle>* triangles = new std::vector<Triangle>();
};