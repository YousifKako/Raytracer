#pragma once

#include <Vector3D.hpp>

class Object3D
{
private:
    std::string object_name = "";
    Vector3D<double> cords = { };
    Vector3D<double> rgb   = { };

protected:
    double specular   = -1;
    double reflective = -1;

public:
    Object3D() { }
    ~Object3D() { }

    void set_cords(const double& x1, const double& x2, const double& x3);
    void set_rgb(const double& x1, const double& x2, const double& x3);
    void set_object_name(const std::string& object_name);

    const Vector3D<double>& get_cords() const;
    const Vector3D<double>& get_rgb() const;
    const std::string& get_object_name() const;


    void set_specular(const double& specular);
    void set_reflective(const double& reflective);

    const double& get_specular() const;
    const double& get_reflective() const;
};