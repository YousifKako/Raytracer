#pragma once

#include <Objects/Object3D.hpp>

class Sphere : public Object3D
{
private:
    const double radius =  1;

public:
    Sphere() { }
    Sphere(const double& radius) : radius(radius) { }
    ~Sphere();

    const double& get_radius() const;
};