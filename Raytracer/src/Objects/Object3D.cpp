#include <Objects/Object3D.hpp>

Object3D::~Object3D()
{
    delete this->cords;
    delete this->rgb;
}

void Object3D::set_cords(const int32_t x, const int32_t y, const int32_t z)
{
    this->cords = new Vector3D<int32_t>(x, y, z);
}

const Vector3D<int32_t>& Object3D::get_cords() const
{
    return *this->cords;
}

void Object3D::set_rgb(const uint16_t r, const uint16_t g, const uint16_t b)
{
    this->rgb = new Vector3D<uint16_t>(r, g, b);
}

const Vector3D<uint16_t>& Object3D::get_rgb() const
{
    return *this->rgb;
}