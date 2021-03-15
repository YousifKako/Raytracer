#include <iostream>
#include <vector>

#include <windows.h>

#include <Objects/Sphere.hpp>
#include <Objects/Light.hpp>

#define MAX_NUM INFINITY

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

const Vector3D<double> canvas_to_viewport(const double x, const double y)
{
    return Vector3D<double>(x * VW / CW, y * VH / CH, distance);
}

const quadratic intersect_ray_sphere(const Vector3D<double>& O,
                                     const Vector3D<double>& D,
                                     const Sphere& sphere)
{
    const double r            = sphere.get_radius();
    const Vector3D<double> CO = O - sphere.get_cords();

    const double a = D * D;
    const double b = 2 * (CO * D);
    const double c = (CO * CO) - (r * r);

    const double discriminant = (b * b) - (4 * (a * c));
    if (discriminant < 0)
        return quadratic{ MAX_NUM, MAX_NUM };

    const double numerator = sqrt(discriminant);
    const double denominator = 2 * a;
    const double t1 = (-b + numerator) / denominator;
    const double t2 = (-b - numerator) / denominator;
    
    return quadratic{ t1, t2 };
}

const double compute_lighting(const Vector3D<double>& P,
                              const Vector3D<double>& N,
                              const Vector3D<double>& V,
                              const double& s,
                              const std::vector<Light*>& lights)
{
    double           i = 0.0;
    Vector3D<double> L;

    for (Light* light : lights)
    {
        if (light->get_type() == "ambient")
            i += light->get_intensity();
        else
        {
            if (light->get_type() == "point")
                L = *light->get_position() - P;
            else
                L = *light->get_direction();

            double n_dot_l = N * L;
            if (n_dot_l > 0)
                i += light->get_intensity() * (n_dot_l / (N.length() * L.length()));

            /*if (s != -1)
            {
                auto R = 2 * N * N * L - L;
                auto r_dot_v = R * V;
                if (r_dot_v >= 0)
                    i += light->get_intensity() * pow(r_dot_v / (R.length() * V.length()), s);
            }*/
        }
    }

    return i;
}

const Vector3D<double> trace_ray(const Vector3D<double>& O,
                                const Vector3D<double>& D,
                                const double& t_min,
                                const double& t_max,
                                const std::vector<Sphere*>* spheres,
                                const std::vector<Light*>* lights)
{
    double closest_t = t_max;
    Sphere* closest_sphere = nullptr;
    for (Sphere* const sphere : *spheres)
    {
        quadratic quads = intersect_ray_sphere(O, D, *sphere);
        if (((quads.t1 >= t_min && quads.t1 <= t_max) && quads.t1 < closest_t) &&
           !(((quads.t2 >= t_min && quads.t2 <= t_max) && quads.t2 < closest_t)))
        {
            closest_t = quads.t1;
            closest_sphere = sphere;
        }
        /*if ((quads.t2 >= t_min && quads.t2 <= t_max) && quads.t2 < closest_t)
        {
            closest_t = quads.t2;
            closest_sphere = sphere;
        }*/
    }

    if (closest_sphere == nullptr)
        return Vector3D<double>(180, 180, 180);

    Vector3D<double> P = O + (closest_t * D);
    Vector3D<double> N = P - (*closest_sphere).get_center();
    N = N / N.length();
    return (*closest_sphere).get_rgb() * compute_lighting(P, N, -D, (*closest_sphere).get_specular(), *lights);
}

int main(int argc, const char* argv[])
{
    // Scene
    Sphere* const sphere1 = new Sphere(45);
    Sphere* const sphere2 = new Sphere(45);
    Sphere* const sphere3 = new Sphere(45);
    Sphere* const sphere4 = new Sphere(5000);
    // TODO: Check for conversions from positive to negitave
    sphere1->set_cords(0, 60, 8);
    sphere1->set_rgb(255, 0, 0);
    sphere4->set_specular(500);

    sphere2->set_cords(-75, 0, 10);
    sphere2->set_rgb(0, 255, 0);
    sphere4->set_specular(10);

    sphere3->set_cords(75, 0, 10);
    sphere3->set_rgb(0, 0, 255);
    sphere4->set_specular(500);

    sphere4->set_cords(0, 5001, 0);
    sphere4->set_rgb(255, 255, 0);
    sphere4->set_specular(1000);

    std::vector<Sphere*>* const spheres = new std::vector<Sphere*>();
    spheres->push_back(sphere1);
    spheres->push_back(sphere2);
    spheres->push_back(sphere3);
    spheres->push_back(sphere4);

    Light* const light1 = new Light("ambient");
    light1->set_intensity(0.3);

    Light* const light2 = new Light("point");
    light2->set_intensity(0.9);
    light2->set_position(new Vector3D<double>(-115, 0, 0));

    Light* const light3 = new Light("directional");
    light3->set_intensity(0.3);
    light3->set_direction(new Vector3D<double>(20, 20, 0));

    std::vector<Light*>* const lights = new std::vector<Light*>();
    lights->push_back(light1);
    lights->push_back(light2);
    lights->push_back(light3);

    const int16_t cw = CW / 2;
    const int16_t ch = CH / 2;

    //HDC hdc = GetDC(NULL);
    HDC hdc = GetDC(GetConsoleWindow());

    Vector3D<double> O = { 0, 0, 0 };
    for (int32_t x = -cw; x < cw; ++x)
    {
        for (int32_t y = -ch; y < ch; ++y)
        {
            const Vector3D<double> D = canvas_to_viewport(x, y);
            const Vector3D<double> color = trace_ray(O, D, distance, MAX_NUM, spheres, lights);
            SetPixel(hdc, x + ch, y + cw, RGB(color[0], color[1], color[2]));
        }
    }
}