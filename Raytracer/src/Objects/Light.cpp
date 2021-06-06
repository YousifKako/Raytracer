#include <Objects/Light.hpp>

Light::~Light()
{
    this->Object3D::~Object3D();
}

void Light::set_intensity(double intensity)
{
    this->intensity = intensity;
}

void Light::set_position(const double& x1, const double& x2, const double& x3)
{
    this->set_cords(x1, x2, x3);
}

void Light::set_direction(const double& x1, const double& x2, const double& x3)
{
    this->set_cords(x1, x2, x3);
}

const double& Light::get_intensity() const
{
    return this->intensity;
}

const std::string& Light::get_type() const
{
    return this->type;
}

const Vector3D<double>&
Light::get_position() const
{
    return this->get_cords();
}

const Vector3D<double>&
Light::get_direction() const
{
    return this->get_cords();
}