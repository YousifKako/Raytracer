#pragma once

#include <vector>

#include <Objects/Object3D.hpp>
#include <Objects/Triangle.hpp>

class Mesh : public Object3D
{
public:
    std::vector<Triangle>* triangles = new std::vector<Triangle>();
};