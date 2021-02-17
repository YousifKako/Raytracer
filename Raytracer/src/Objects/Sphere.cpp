#include <Objects/Sphere.hpp>

Sphere::~Sphere()
{
    this->Object3D::~Object3D();
}

const uint64_t Sphere::get_radius() const
{
    return this->radius;
}