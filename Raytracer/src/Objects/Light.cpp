#include <Objects/Light.hpp>

void Light::set_intensity(double intensity)
{
    this->intensity = intensity;
}

void Light::set_position(Vector3D<double>* position)
{
    this->position = position;
}

void Light::set_direction(Vector3D<double>* direction)
{
    this->direction = direction;
}

const double& Light::get_intensity()
{
    return this->intensity;
}

const std::string& Light::get_type() const
{
    return this->type;
}

const Vector3D<double>* Light::get_position() const
{
    return this->position;
}

const Vector3D<double>* Light::get_direction() const
{
    return this->direction;
}