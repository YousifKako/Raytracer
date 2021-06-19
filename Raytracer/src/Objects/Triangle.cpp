#include <Objects/Triangle.hpp>

void Triangle::set_vertices(const Vector3D<double> x1,
                            const Vector3D<double> x2,
                            const Vector3D<double> x3)
{
    this->vertices.change_vars(x1, x2, x3);
}

void Triangle::set_vertices_rgb(const Vector3D<double> x1,
                                const Vector3D<double> x2,
                                const Vector3D<double> x3)
{
    this->vertices_rgb.change_vars(x1, x2, x3);
}

void Triangle::set_vertices_rgb(double x1, double x2, double x3)
{
    this->normals.change_vars(x1, x2, x3);
}

void Triangle::set_normals(const Vector3D<double> normals)
{
    this->normals.change_vars(normals[0], normals[1], normals[2]);
}

const Vector3D<Vector3D<double>>&
Triangle::get_vertices() const
{
    return this->vertices;
}

const Vector3D<Vector3D<double>>&
Triangle::get_vertices_rgb() const
{
    return this->vertices_rgb;
}

const Vector3D<double>&
Triangle::get_normals() const
{
    return this->normals;
}