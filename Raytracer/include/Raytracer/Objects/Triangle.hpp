#pragma once

#include <Vector3D.hpp>
#include <Objects/Object3D.hpp>

class Triangle : public Object3D
{
private:
    Vector3D<Vector3D<double>> vertices = { };
    Vector3D<Vector3D<double>> vertices_rgb = { };
    Vector3D<double> normals = { };

public:
    void set_vertices(const Vector3D<double>& x1,
                      const Vector3D<double>& x2,
                      const Vector3D<double>& x3);
    void set_vertices_rgb(const Vector3D<double>& x1,
                          const Vector3D<double>& x2,
                          const Vector3D<double>& x3);
    void set_vertices_rgb(double x1, double x2, double x3);
    void set_normals(const Vector3D<double>& normals);
    
    const Vector3D<Vector3D<double>>& get_vertices() const;
    const Vector3D<Vector3D<double>>& get_vertices_rgb() const;
    const Vector3D<double>& get_normals() const;
};