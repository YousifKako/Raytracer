#include <Objects/Sphere.hpp>

Sphere::~Sphere()
{
    this->Object3D::~Object3D();
}

void Sphere::set_specular(const double& specular)
{
    this->specular = specular;
}
const double& Sphere::get_specular() const
{
    return this->specular;
}

const double& Sphere::get_radius() const
{
    return this->radius;
}