#include <iostream>
#include <vector>

#include <windows.h>

#include <Objects/Sphere.hpp>

#define MAX_NUM 100000

const int16_t CW       = 500;
const int16_t CH       = 500;
const int16_t VW       = 200;
const int16_t VH       = 200;
const int16_t distance = 1;

typedef struct quadratic
{
    const double t1 = NULL;
    const double t2 = NULL;
} quadratic;

const Vector3D<int16_t> canvas_to_viewport(const int16_t x, const int16_t y)
{
    return Vector3D<int16_t>(x * VW / CW, y * VH / CH, distance);
}

const quadratic intersect_ray_sphere(const Vector3D<int32_t>& O,
    const Vector3D<int16_t>& D, const Sphere& sphere)
{
    const uint64_t r           = sphere.get_radius();
    const Vector3D<int32_t> CO = O - sphere.get_cords();

    const int64_t a = D * D;
    const int64_t b = 2 * (CO * D);
    const int64_t c = (CO * CO) - (r * r);

    const size_t discriminant = (b * b) - (4 * (a * c));
    if (discriminant < 0)
        return quadratic{ MAX_NUM, MAX_NUM };

    const double numerator = sqrt(discriminant);
    const int64_t denominator = 2 * a;
    const double t1 = (-b + numerator) / denominator;
    const double t2 = (-b - numerator) / denominator;
    
    return quadratic{ t1, t2 };
}

const Vector3D<uint16_t> trace_ray(const Vector3D<int32_t>& O, const Vector3D<int16_t>& D,
                                   const double t_min, const double t_max, const std::vector<Sphere*>& spheres)
{
    for (const Sphere* const sphere : spheres)
    {
        quadratic quads = intersect_ray_sphere(O, D, *sphere);
        if ((quads.t1 >= t_min && quads.t1 <= t_max) && !(quads.t2 >= t_min && quads.t2 <= t_max))
            return (*sphere).get_rgb();
    }

    return Vector3D<uint16_t>(0, 0, 0);
}

int main(int argc, const char* argv[])
{
    // Scene
    Sphere* const sphere1 = new Sphere(30);
    Sphere* const sphere2 = new Sphere(30);
    Sphere* const sphere3 = new Sphere(30);

    sphere1->set_cords(0, 45, 1);
    sphere1->set_rgb(255, 0, 0);

    sphere2->set_cords(-45, 0, 10);
    sphere2->set_rgb(0, 255, 0);

    sphere3->set_cords(45, 0, 10);
    sphere3->set_rgb(0, 0, 255);

    std::vector<Sphere*>* spheres = new std::vector<Sphere*>();
    spheres->push_back(sphere1);
    spheres->push_back(sphere2);
    spheres->push_back(sphere3);

    const int16_t cw = CW / 2;
    const int16_t ch = CH / 2;

    //HDC hdc = GetDC(NULL);
    HDC hdc = GetDC(GetConsoleWindow());

    Vector3D<int32_t> O = { 0, 0, 0 };
    for (int16_t x = -cw; x < cw; ++x)
    {
        for (int16_t y = -ch; y < ch; ++y)
        {
            const Vector3D<int16_t> D = canvas_to_viewport(x, y);
            const Vector3D<uint16_t> color = trace_ray(O, D, distance, MAX_NUM, *spheres);
            SetPixel(hdc, x+ch, y+cw, RGB(color[0], color[1], color[2]));
        }
    }
}