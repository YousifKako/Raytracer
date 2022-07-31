#pragma once

#include <Vector3D.hpp>

class Object3D
{
private:
    Vector3D<double> cords = { };
    Vector3D<double> rgb   = { };

protected:
    double specular   = -1;
    double reflective = -1;

public:
    Object3D() { }
    ~Object3D() { }

    void set_cords(const double x1, const double x2, const double x3);
    void set_cords(const Vector3D<double>& temp);
    void set_rgb(const double x1, const double x2, const double x3);
    void set_rgb(const Vector3D<double>& temp);

    const Vector3D<double>& get_cords() const;
    const Vector3D<double>& get_rgb() const;


    void set_specular(const double specular);
    void set_reflective(const double reflective);

    const double get_specular() const;
    const double get_reflective() const;
};