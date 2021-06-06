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

const auto&
Triangle::get_vertices() const
{
    return this->vertices;
}

const auto&
Triangle::get_vertices_rgb() const
{
    return this->vertices_rgb;
}