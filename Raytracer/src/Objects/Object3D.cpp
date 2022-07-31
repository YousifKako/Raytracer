#include <Objects/Object3D.hpp>

void
Object3D::set_cords(const double x1, const double x2, const double x3)
{
    this->cords.change_vars(x1, x2, x3);
}

void
Object3D::set_cords(const Vector3D<double>& temp)
{
    this->cords.change_vars(temp);
}

void
Object3D::set_rgb(const double x1, const double x2, const double x3)
{
    this->rgb.change_vars(x1, x2, x3);
}

void
Object3D::set_rgb(const Vector3D<double>& temp)
{
    this->rgb.change_vars(temp);
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

void
Object3D::set_specular(const double specular)
{
    this->specular = specular;
}

void
Object3D::set_reflective(const double reflective)
{
    this->reflective = reflective;
}

const double
Object3D::get_specular() const
{
    return this->specular;
}

const double
Object3D::get_reflective() const
{
    return this->reflective;
}