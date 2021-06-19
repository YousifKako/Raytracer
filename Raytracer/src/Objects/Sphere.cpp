#include <Objects/Sphere.hpp>

Sphere::~Sphere()
{
    this->Object3D::~Object3D();
}

const double&
Sphere::get_radius() const
{
    return this->radius;
}