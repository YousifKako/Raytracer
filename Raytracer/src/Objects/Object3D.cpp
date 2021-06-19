#include <Objects/Object3D.hpp>

void Object3D::set_cords(const double& x1, const double& x2, const double& x3)
{
    this->cords.change_vars(x1, x2, x3);
}

void Object3D::set_rgb(const double& x1, const double& x2, const double& x3)
{
    this->rgb.change_vars(x1, x2, x3);
}

void Object3D::set_object_name(const std::string& object_name)
{
    this->object_name = object_name;
}

const Vector3D<double>&
Object3D::get_cords() const
{
    return this->cords;
}

const Vector3D<double>&
Object3D::get_rgb() const
{
    return this->rgb;
}

const std::string&
Object3D::get_object_name() const
{
    return this->object_name;
}


void Object3D::set_specular(const double& specular)
{
    this->specular = specular;
}

void Object3D::set_reflective(const double& reflective)
{
    this->reflective = reflective;
}

const double&
Object3D::get_specular() const
{
    return this->specular;
}

const double&
Object3D::get_reflective() const
{
    return this->reflective;
}