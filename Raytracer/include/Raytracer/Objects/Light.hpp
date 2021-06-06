#pragma once

#include <Objects/Object3D.hpp>

class Light : public Object3D
{
private:
    const std::string type = "";
    double intensity = 0.f;

public:
    Light() = delete;
    Light(std::string type) : type(type) { }
    ~Light();

    void set_intensity(double intensity);
    void set_position(const double& x1, const double& x2, const double& x3);
    void set_direction(const double& x1, const double& x2, const double& x3);

    const double& get_intensity() const;
    const std::string& get_type() const;
    const Vector3D<double>& get_position() const;
    const Vector3D<double>& get_direction() const;
};