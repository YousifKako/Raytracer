#pragma once

#include <Objects/Object3D.hpp>

class Sphere : public Object3D
{
private:
    double radius =  1;

public:
    Sphere() { }
    Sphere(const double radius) : radius(radius) { }
    ~Sphere();

    const double get_radius() const;

    void
    operator=(const Sphere& temp)
    {
        radius = temp.get_radius();
        set_cords(temp.get_cords());
        set_rgb(temp.get_rgb());
        set_specular(temp.get_specular());
        set_reflective(temp.get_reflective());
    }
};