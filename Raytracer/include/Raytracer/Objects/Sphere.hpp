#pragma once

#include <Objects/Object3D.hpp>

class Sphere : public Object3D
{
private:
    const double radius =  1;
    double specular     = -1;

public:
    Sphere() { }
    Sphere(const double& radius) : radius(radius) { }
    ~Sphere();

    void set_specular(const double& specular);

    const double& get_specular() const;
    const double& get_radius() const;
};