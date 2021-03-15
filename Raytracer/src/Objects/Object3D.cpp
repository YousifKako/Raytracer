#include <Objects/Object3D.hpp>

Object3D::~Object3D()
{
    delete this->cords;
    delete this->rgb;
}

void Object3D::set_cords(const double& x, const double& y, const double& z)
{
    this->cords = new Vector3D<double>(x, y, z);
}

void Object3D::set_rgb(const float& r, const float& g, const float& b)
{
    this->rgb = new Vector3D<float>(r, g, b);
}

const Vector3D<double>& Object3D::get_cords() const
{
    return *this->cords;
}

const Vector3D<float>& Object3D::get_rgb() const
{
    return *this->rgb;
}