#pragma once

#include <Vector3D.hpp>

class Object3D
{
private:
    Vector3D<double> cords = { };
    Vector3D<double> rgb   = { };

public:
    Object3D() { }
    ~Object3D() { }

    void set_cords(const double& x1, const double& x2, const double& x3);
    void set_rgb(const double& x1, const double& x2, const double& x3);

    const Vector3D<double>& get_cords() const;
    const Vector3D<double>& get_rgb() const;
};