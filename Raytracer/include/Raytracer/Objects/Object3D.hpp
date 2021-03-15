#pragma once

#include <Vector3D.hpp>

class Object3D
{
private:
    Vector3D<double>* cords = nullptr;
    Vector3D<float>*  rgb   = nullptr;

public:
    Object3D() { }
    ~Object3D();

    void set_cords(const double& x, const double& y, const double& z);
    void set_rgb(const float& r, const float& g, const float& b);

    const Vector3D<double>& get_cords() const;
    const Vector3D<float>& get_rgb() const;
};