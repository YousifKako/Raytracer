#include <iostream>
#include <vector>

#include <windows.h>

#include <Objects/Sphere.hpp>
#include <Objects/Light.hpp>

#define PI                  3.14159265358979323846
#define EPSILON             0.0001
#define MAX_RENDER_DISTANCE INFINITY

const double CW       = 500;
const double CH       = 500;
const double VW       = 1;
const double VH       = 1;
const double distance = 1;

const Vector3D<double>
canvas_to_viewport(const double& x, const double& y)
{
    return Vector3D<double>(x * VW / CW, y * VH / CH, distance);
}

const std::pair<const double&, const double&>&
intersect_ray_sphere(const Vector3D<double>& O,
                     const Vector3D<double>& D,
                     const Sphere& sphere)
{
    const double r            = sphere.get_radius();
    const Vector3D<double> CO = O - sphere.get_cords();

    const double a = D * D;
    const double b = 2 * (CO * D);
    const double c = (CO * CO) - (r * r);

    const double discriminant = (b * b) - ((4 * a) * c);
    if (discriminant < 0)
        return std::pair{ MAX_RENDER_DISTANCE, MAX_RENDER_DISTANCE };

    const double numerator   = std::sqrt(discriminant);
    const double denominator = 2 * a;
    const double t1          = (-b + numerator) / denominator;
    const double t2          = (-b - numerator) / denominator;

    return std::pair<const double&, const double&>(t1, t2);
}


const std::pair<const Sphere*, const double&>&
closest_intersection(const Vector3D<double>& O,
                     const Vector3D<double>& D,
                     const double& t_min,
                     const double& t_max,
                     const std::vector<Sphere*>* spheres)
{
    double closest_t             = t_max;
    const Sphere* closest_sphere = nullptr;
    for (Sphere* const sphere : *spheres)
    {
        const auto quads = intersect_ray_sphere(O, D, *sphere);
        const double t1  = quads.first;
        const double t2  = quads.second;

        if ((t1 >= t_min && t1 <= t_max) && t1 < closest_t)
        {
            closest_t      = t1;
            closest_sphere = sphere;
        }
        if ((t2 >= t_min && t2 <= t_max) && t2 < closest_t)
        {
            closest_t      = t2;
            closest_sphere = sphere;
        }
    }

    return std::pair<const Sphere*, const double&>(closest_sphere, closest_t);
}

const Vector3D<double>&
reflect_ray(const Vector3D<double>& R, const Vector3D<double>& N)
{
    return 2 * N * (N * R) - R;
}

const double&
compute_lighting(const Vector3D<double>& P,
                 const Vector3D<double>& N,
                 const Vector3D<double>& V,
                 const double& s,
                 const std::vector<Sphere*>* spheres,
                 const std::vector<Light*>* lights)
{
    double               i = 0.0;
    double           t_max = 1;
    Vector3D<double>     L = { };

    for (Light* light : *lights)
    {
        if (light->get_type() == "ambient")
            i += light->get_intensity();
        else
        {
            if (light->get_type() == "point")
            {
                L = *light->get_position() - P;
                t_max = 1;
            }
            else
            {
                L = *light->get_direction();
                t_max = MAX_RENDER_DISTANCE;
            }

            // Shadows
            const auto shadow = closest_intersection(P, L, EPSILON, t_max, spheres);
            if (shadow.first != nullptr)
                continue;

            // Diffuse
            const double n_dot_l = N * L;
            if (n_dot_l > 0)
                i += light->get_intensity() * (n_dot_l / (N.length() * L.length()));

            // Specular
            if (s != -1)
            {
                const Vector3D<double> R = reflect_ray(L, N);
                const double r_dot_v     = R * V;
                if (r_dot_v > 0)
                    i += light->get_intensity() * std::pow(r_dot_v / (R.length() * V.length()), s);
            }
        }
    }

    return i;
}

const Vector3D<double>&
trace_ray(const Vector3D<double>& O,
          const Vector3D<double>& D,
          const double& t_min,
          const double& t_max,
          const double& recursion_depth,
          const std::vector<Sphere*>* spheres,
          const std::vector<Light*>* lights)
{
    const auto intersect_pair = closest_intersection(O, D, t_min, t_max, spheres);
    const Sphere* closest_sphere = intersect_pair.first;
    const double  closest_t      = intersect_pair.second;

    if (closest_sphere == nullptr)
        return Vector3D<double>(0, 0, 0);

    const Vector3D<double> P = O + (closest_t * D);
    Vector3D<double> N       = P - (*closest_sphere).get_cords();
    N = N / N.length();
    const Vector3D<double> local_color = (*closest_sphere).get_rgb() *
            compute_lighting(P, N, -D, (*closest_sphere).get_specular(), spheres, lights);

    const double r = (*closest_sphere).get_reflective();
    if (recursion_depth <= 0 || r <= 0)
        return local_color;

    const Vector3D<double> R               = reflect_ray(-D, N);
    const Vector3D<double> reflected_color = trace_ray(P, R, EPSILON, t_max,
                                                    recursion_depth-1, spheres, lights);

    return local_color * (1 - r) + reflected_color * r;
}

const Vector3D<Vector3D<double>>&
compute_rotation_matrix(const Vector3D<double>& vec)
{
    const double x0   = vec[0] * PI / 180.0;
    const double x1   = vec[1] * PI / 180.0;
    const double x2   = vec[2] * PI / 180.0;

    const double sinX = std::sin(x0);
    const double sinY = std::sin(x1);
    const double sinZ = std::sin(x2);

    const double cosX = std::cos(x0);
    const double cosY = std::cos(x1);
    const double cosZ = std::cos(x2);

    const Vector3D<Vector3D<double>> rotation = {
        {cosZ*cosY, cosZ*sinY*sinX-sinZ*cosX, cosZ*sinY*cosX+sinZ*sinX},
        {sinZ*cosY, sinZ*sinY*sinX+cosZ*cosX, sinZ*sinY*cosX-cosZ*sinX},
        {    -sinY,                cosY*sinX,                cosY*cosX}};

    return rotation;
}

int main(int argc, const char* argv[])
{
    // Scene
    // Spheres
    Sphere* const sphere1 = new Sphere(2);
    sphere1->set_cords(0, -2, 7);
    sphere1->set_rgb(255, 0, 0);
    sphere1->set_specular(500);
    sphere1->set_reflective(0.2);

    Sphere* const sphere2 = new Sphere(2);
    sphere2->set_cords(-3.5, 0, 9);
    sphere2->set_rgb(0, 255, 0);
    sphere2->set_specular(10);
    sphere2->set_reflective(0.4);

    Sphere* const sphere3 = new Sphere(2);
    sphere3->set_cords(3.5, 0, 9);
    sphere3->set_rgb(0, 0, 255);
    sphere3->set_specular(500);
    sphere3->set_reflective(0.3);

    Sphere* const sphere4 = new Sphere(5000);
    sphere4->set_cords(0, -5002, 0);
    sphere4->set_rgb(255, 255, 0);
    sphere4->set_specular(1000);
    sphere4->set_reflective(0.4);

    std::vector<Sphere*>* const spheres = new std::vector<Sphere*>();
    spheres->push_back(sphere1);
    spheres->push_back(sphere2);
    spheres->push_back(sphere3);
    spheres->push_back(sphere4);
    
    // Lights
    Light* const light1 = new Light("ambient");
    light1->set_intensity(0.3);
    
    Light* const light2 = new Light("point");
    light2->set_intensity(0.4);
    light2->set_position(new Vector3D<double>(2, 0, 0));
    
    Light* const light3 = new Light("directional");
    light3->set_intensity(0.2);
    light3->set_direction(new Vector3D<double>(1, 4, 4));
    
    std::vector<Light*>* const lights = new std::vector<Light*>();
    lights->push_back(light1);
    lights->push_back(light2);
    lights->push_back(light3);

    const int16_t cw = CW / 2;
    const int16_t ch = CH / 2;

    //const HDC hdc = GetDC(NULL);
    const HDC hdc = GetDC(GetConsoleWindow());

    Vector3D<double> O           = { -5, 0, 2 };
    const double recursion_depth = 3;
    auto rotation                = compute_rotation_matrix({ 0, 40, 0 });

    for (int32_t x = -cw; x < cw; ++x)
    {
        for (int32_t y = -ch; y < ch; ++y)
        {
            const Vector3D<double> D     = rotation * canvas_to_viewport(x, y);
            const Vector3D<double> color = trace_ray(O, D, distance, MAX_RENDER_DISTANCE,
                                                     recursion_depth, spheres, lights);
            SetPixel(hdc, x + ch, -y + cw, RGB(color[0], color[1], color[2]));
        }
    }
}