#pragma once

#include <Vector3D.hpp>

class Triangle
{
private:
    Vector3D<Vector3D<double>> vertices = { };
    Vector3D<Vector3D<double>> vertices_rgb = { };

public:
    void set_vertices(const Vector3D<double> x1,
                      const Vector3D<double> x2,
                      const Vector3D<double> x3);
    void set_vertices_rgb(const Vector3D<double> x1,
                          const Vector3D<double> x2,
                          const Vector3D<double> x3);
    
    const auto& get_vertices() const;
    const auto& get_vertices_rgb() const;
};