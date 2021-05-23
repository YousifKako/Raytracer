#pragma once

#include <Vector3D.hpp>

class Light
{
private:
    const std::string type = "";
    double intensity = 0.0;
    Vector3D<double>* position = nullptr;
    Vector3D<double>* direction = nullptr;

public:
    Light() = delete;
    Light(std::string type) : type(type) { }
    ~Light() { };

    void set_intensity(double intensity);
    void set_position(Vector3D<double>* position);
    void set_direction(Vector3D<double>* direction);

    const double& get_intensity() const;
    const std::string& get_type() const;
    const Vector3D<double>* get_position() const;
    const Vector3D<double>* get_direction() const;
};