#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <mutex>
#include <windows.h>

#include <Objects/Light.hpp>
#include <Objects/Triangle.hpp>
#include <Objects/Sphere.hpp>
#include <Model/Mesh.hpp>
#include <Model/Model.hpp>
#include <Utilities.hpp>

#include <GLFW/glfw3.h>


#define NUM_THREADS 4

namespace Sphere_RayTrace {
    const std::pair<const double, const double>
    intersect_ray_sphere(const Vector3D<double>& O,
                         const Vector3D<double>& D,
                         const Sphere& sphere)
    {
        const double r = sphere.get_radius();
        const Vector3D<double> CO = O - sphere.get_cords();

        const double a = D * D;
        const double b = 2 * (CO * D);
        const double c = (CO * CO) - (r * r);

        const double discriminant = (b * b) - ((4 * a) * c);
        if (discriminant < 0)
            return std::pair<const double, const double>(MAX_RENDER_DISTANCE, MAX_RENDER_DISTANCE);

        const double numerator = std::sqrt(discriminant);
        const double denominator = 2 * a;
        const double t1 = (-b + numerator) / denominator;
        const double t2 = (-b - numerator) / denominator;

        return std::pair<const double, const double>(t1, t2);
    }

    const std::tuple<const bool, const Sphere, const double>
    closest_intersection(const Vector3D<double>& O,
                         const Vector3D<double>& D,
                         const double t_min,
                         const double t_max,
                         const std::vector<Sphere>* spheres)
    {
        double closest_t = t_max;
        bool intersects = false;
        Sphere closest_sphere = { };
        for (const Sphere& sphere : *spheres)
        {
            const auto quads = intersect_ray_sphere(O, D, sphere);
            const double t1 = quads.first;
            const double t2 = quads.second;

            if ((t1 >= t_min && t1 <= t_max) && t1 < closest_t)
            {
                closest_t = t1;
                closest_sphere = sphere;
                intersects = true;
            }
            if ((t2 >= t_min && t2 <= t_max) && t2 < closest_t)
            {
                closest_t = t2;
                closest_sphere = sphere;
                intersects = true;
            }
        }

        return std::tuple<const bool, const Sphere, const double>(intersects, closest_sphere, closest_t);
    }

    const double
    compute_lighting(const Vector3D<double>& P,
                     const Vector3D<double>& N,
                     const Vector3D<double>& V,
                     const double s,
                     const std::vector<Sphere>* spheres,
                     const std::vector<Light>* lights)
    {
        double i = 0.0;
        double t_max = 1;
        Vector3D<double> L = { };

        for (const Light& light : *lights)
        {
            // Clamp light intensity so it doesn't go over 255 (1.f)
            if (i > 1.f)
                i = 1.f;

            if (light.get_type() == "ambient")
                i += light.get_intensity();
            else
            {
                if (light.get_type() == "point")
                {
                    L = light.get_cords() - P;
                    t_max = 1;
                }
                else
                {
                    L = light.get_cords();
                    t_max = MAX_RENDER_DISTANCE;
                }

                // Shadows
                const auto shadow = closest_intersection(P, L, EPSILON, t_max, spheres);
                if (std::get<0>(shadow) != false)
                    continue;

                // Diffuse Reflection
                const double n_dot_l = N * L;
                if (n_dot_l > 0)
                {
                    i += light.get_intensity() * (n_dot_l / (N.length() * L.length()));
                    if (i > 1.f)
                        i = 1.f;
                }

                // Specular Reflection
                if (s != -1)
                {
                    const Vector3D<double> R = reflect_ray(L, N);
                    const double r_dot_v = R * V;
                    if (r_dot_v > 0)
                        i += light.get_intensity() * std::pow(r_dot_v / (R.length() * V.length()), s);
                    if (i > 1.f)
                        i = 1.f;
                }
            }
        }

        return i;
    }

    const Vector3D<double>
    trace_ray(const Vector3D<double>& O,
              const Vector3D<double>& D,
              const double t_min,
              const double t_max,
              const double recursion_depth,
              const std::vector<Mesh>* const model,
              const std::vector<Sphere>* spheres,
              const std::vector<Light>* lights)
    {
        const auto intersect_sphere = Sphere_RayTrace::closest_intersection(O, D, t_min, t_max, spheres);
        const bool closest_intersect = std::get<0>(intersect_sphere);
        const Sphere closest_sphere = std::get<1>(intersect_sphere);
        const double closest_sphere_t = std::get<2>(intersect_sphere);

        if (!closest_intersect)
            return BACKGROUND_COLOR;

        const Vector3D<double> P = O + (closest_sphere_t * D);
        Vector3D<double> N = P - closest_sphere.get_cords();
        N = N / N.length();
        const Vector3D<double> local_color = closest_sphere.get_rgb() *
            Sphere_RayTrace::compute_lighting(P, N, -D, closest_sphere.get_specular(), spheres, lights);

        const double r = closest_sphere.get_reflective();
        if (recursion_depth <= 0 || r <= 0)
            return local_color;

        const Vector3D<double> R = reflect_ray(-D, N);
        const Vector3D<double> reflected_color = Sphere_RayTrace::trace_ray(P, R, EPSILON, t_max, recursion_depth - 1, model, spheres, lights);

        return local_color * (1 - r) + reflected_color * r;
    }
}

namespace Triangle_RayTrace {
    inline const bool
    moller_trumbore(Vector3D<double>& coords,
                    const Vector3D<double>& O,
                    const Vector3D<double>& D,
                    const Vector3D<Vector3D<double>>& V)
    {
        const Vector3D<double> E1 = V[1] - V[0];
        const Vector3D<double> E2 = V[2] - V[0];
        const Vector3D<double> P = D.cross(E2);

        const double det = (P * E1);

        // Check if Ray and Plane (Triangle) are parallel
        if (det < EPSILON)
            return false;

        const Vector3D<double> T = O - V[0];
        double u = T * P;
        if (u < 0.f || u > det)
            return false;

        const Vector3D<double> Q = T.cross(E1);
        double v = D * Q;
        if (v < 0.f || u + v > det)
            return false;

        double t = E2 * Q;
        if (t < EPSILON)
            return false;

        const double det_inv = 1.f / det;
        t *= det_inv;
        u *= det_inv;
        v *= det_inv;

        coords.change_vars(t, u, v);

        return true;
    }

    const std::tuple<bool, double, const Triangle>
    closest_intersection(const Vector3D<double>& O,
                         const Vector3D<double>& D,
                         const double t_min,
                         const double t_max,
                         Vector3D<double>& coords,
                         const std::vector<Mesh>* const model)
    {
        double closest_t = MAX_RENDER_DISTANCE;
        Triangle closest_triangle;
        bool intersect = false;

        for (const Mesh& mesh : *model)
        {
            for (const Triangle& triangle : *mesh.triangles)
            {
                const auto V = triangle.get_vertices();
                const auto N = triangle.get_normals();

                if (moller_trumbore(coords, O, D, V))
                {
                    if (closest_t >= t_min && closest_t <= t_max && coords[0] < closest_t)
                    {
                        closest_t = coords[0];
                        closest_triangle = triangle;
                        intersect = true;
                    }
                }
            }
        }

        return std::tuple<bool, double, const Triangle>(intersect, closest_t, closest_triangle);
    }

    const Vector3D<double>
    get_color(const Vector3D<double>& baryc, const Triangle& triangle)
    {
        auto u = baryc[1];
        auto v = baryc[2];
        auto w = 1 - u - v;

        auto rgb = triangle.get_vertices_rgb();

        return (w * rgb[0]) * 255 + (u * rgb[1]) * 255 + (v * rgb[2]) * 255;
    }

    const double
    compute_lighting(const Vector3D<double>& P,
                     const Vector3D<double>& N,
                     const Vector3D<double>& V,
                     const double s,
                     const std::vector<Light>* const lights,
                     const std::vector<Mesh>* const model)
    {
        double i           = 0.f;
        double t_max       = 1;
        Vector3D<double> L = { };

        for (const Light& light : *lights)
        {
            if (light.get_type() == "ambient")
                i += light.get_intensity();
            else
            {
                if (light.get_type() == "point")
                {
                    L = light.get_cords() - P;
                    t_max = 1;
                }
                else
                {
                    L = light.get_cords();
                    t_max = MAX_RENDER_DISTANCE;
                }

                // Shadows
                Vector3D<double> coords = Vector3D<double>();
                const auto shadow = closest_intersection(P, L, EPSILON, t_max, coords, model);
                if (std::get<0>(shadow))
                    continue;

                // Diffuse Reflection
                const double n_dot_l = N * L;
                if (n_dot_l > 0)
                    i += light.get_intensity() * (n_dot_l / (N.length() * L.length()));

                // Specular Reflection
                if (s != -1)
                {
                    const Vector3D<double> R = reflect_ray(L, N);
                    const double r_dot_v = R * V;
                    if (r_dot_v > 0)
                        i += light.get_intensity() * std::pow(r_dot_v / (R.length() * V.length()), s);
                }
            }
        }

        return i;
    }

    const Vector3D<double>
    trace_ray(const Vector3D<double>& O,
              const Vector3D<double>& D,
              const double t_min,
              const double t_max,
              const double recursion_depth,
              const std::vector<Mesh>* const model,
              const std::vector<Sphere>* spheres,
              const std::vector<Light>* lights)
    {
        Vector3D<double> coords = Vector3D<double>();
        const auto intersect_triangle = Triangle_RayTrace::closest_intersection(O, D, t_min, t_max, coords, model);
        const Triangle closest_triangle = std::get<2>(intersect_triangle);

        if (!std::get<0>(intersect_triangle))
            return BACKGROUND_COLOR;

        // Get RBG colors
        auto pixel = Triangle_RayTrace::get_color(coords, closest_triangle);

        const auto P = O + coords[0] * D;
        const Vector3D<double> local_color = pixel * Triangle_RayTrace::compute_lighting(P, closest_triangle.get_normals(), -D, closest_triangle.get_specular(), lights, model);

        const double r = closest_triangle.get_reflective();
        if (recursion_depth <= 0 || r <= 0)
            return local_color;

        const Vector3D<double> R = reflect_ray(-D, closest_triangle.get_normals());
        const Vector3D<double> reflected_color = Triangle_RayTrace::trace_ray(P, R, EPSILON, t_max, recursion_depth - 1, model, spheres, lights);

        return local_color * (1 - r) + reflected_color * r;
    }
}

inline const Vector3D<double>
trace_ray(const Vector3D<double>& O,
          const Vector3D<double>& D,
          const double t_min,
          const double t_max,
          const double recursion_depth,
          const std::vector<Mesh>* const model,
          const std::vector<Sphere>* spheres,
          const std::vector<Light>* lights);

const Vector3D<double>
evaluate_final_triangle_color(const Triangle& closest_triangle,
                              const Vector3D<double>& coords,
                              const Vector3D<double>& O,
                              const Vector3D<double>& D,
                              const double t_max,
                              const double recursion_depth,
                              const std::vector<Mesh>* const model,
                              const std::vector<Sphere>* spheres,
                              const std::vector<Light>* lights)
{
    // Get RBG colors
    auto pixel = Triangle_RayTrace::get_color(coords, closest_triangle);

    const auto P = O + coords[0] * D;
    const Vector3D<double> local_color = pixel * Triangle_RayTrace::compute_lighting(P, closest_triangle.get_normals(), -D, closest_triangle.get_specular(), lights, model);

    const double r = closest_triangle.get_reflective();
    if (recursion_depth <= 0 || r <= 0)
        return local_color;

    const Vector3D<double> R = reflect_ray(-D, closest_triangle.get_normals());
    const Vector3D<double> reflected_color = trace_ray(P, R, EPSILON, t_max, recursion_depth - 1, model, spheres, lights);

    return local_color * (1 - r) + reflected_color * r;
}

const Vector3D<double>
evaluate_final_sphere_color(const Sphere& closest_sphere,
                            const double closest_sphere_t,
                            const Vector3D<double>& O,
                            const Vector3D<double>& D,
                            const double t_max,
                            const double recursion_depth,
                            const std::vector<Mesh>* const model,
                            const std::vector<Sphere>* spheres,
                            const std::vector<Light>* lights)
{
    const Vector3D<double> P = O + (closest_sphere_t * D);
    Vector3D<double> N = P - closest_sphere.get_cords();
    N = N / N.length();
    const Vector3D<double> local_color = closest_sphere.get_rgb() *
        Sphere_RayTrace::compute_lighting(P, N, -D, closest_sphere.get_specular(), spheres, lights);

    const double r = closest_sphere.get_reflective();
    if (recursion_depth <= 0 || r <= 0)
        return local_color;

    const Vector3D<double> R = reflect_ray(-D, N);
    const Vector3D<double> reflected_color = trace_ray(P, R, EPSILON, t_max, recursion_depth - 1, model, spheres, lights);

    return local_color * (1 - r) + reflected_color * r;
}

inline const Vector3D<double>
trace_ray(const Vector3D<double>& O,
          const Vector3D<double>& D,
          const double t_min,
          const double t_max,
          const double recursion_depth,
          const std::vector<Mesh>* const model,
          const std::vector<Sphere>* spheres,
          const std::vector<Light>* lights)
{
    const auto intersect_sphere = Sphere_RayTrace::closest_intersection(O, D, t_min, t_max, spheres);
    const bool closest_intersect = std::get<0>(intersect_sphere);
    const Sphere closest_sphere = std::get<1>(intersect_sphere);
    const double closest_sphere_t = std::get<2>(intersect_sphere);
        
    if (!closest_intersect)
    {
        Vector3D<double> coords = Vector3D<double>();
        const auto intersect_triangle = Triangle_RayTrace::closest_intersection(O, D, t_min, t_max, coords, model);
            
        if (!std::get<0>(intersect_triangle))
            return BACKGROUND_COLOR;
        
        return evaluate_final_triangle_color(std::get<2>(intersect_triangle), coords, O, D, t_max, recursion_depth, model, spheres, lights);
    }

    Vector3D<double> coords = Vector3D<double>();
    auto intersect_triangle = Triangle_RayTrace::closest_intersection(O, D, t_min, t_max, coords, model);
        
    if (!std::get<0>(intersect_triangle))
        return evaluate_final_sphere_color(closest_sphere, closest_sphere_t, O, D, t_max, recursion_depth, model, spheres, lights);
        
    if (std::get<1>(intersect_triangle) < closest_sphere_t)
        return evaluate_final_triangle_color(std::get<2>(intersect_triangle), coords, O, D, t_max, recursion_depth, model, spheres, lights);
        
    return evaluate_final_sphere_color(closest_sphere, closest_sphere_t, O, D, t_max, recursion_depth, model, spheres, lights);
}

namespace Scene {
    typedef struct RunDeps
    {
        int16_t cw = 0;
        int16_t ch = 0;
        Vector3D<double> O = { };
        double recursion_depth = 0;
        Vector3D<Vector3D<double>> rotation = { };

        const std::vector<Mesh>* const model = nullptr;
        const std::vector<Sphere>* const spheres = nullptr;
        const std::vector<Light>* const lights = nullptr;
        unsigned char* pixels = new unsigned char[CW * CH * 4];
    } RunDeps;

    const RunDeps setup()
    {
        RunDeps deps = { };

        Model model = Model("..\\assets\\Test.obj");
        std::vector<Mesh> a = model.get();
        std::vector<Mesh>* mod = new std::vector<Mesh>();
        mod->push_back(a[0]);
        mod->push_back(a[1]);
        mod->push_back(a[2]);

        // Lights
        Light light1 = Light("ambient");
        light1.set_intensity(0.5);

        Light light2 = Light("point");
        light2.set_intensity(0.2);
        light2.set_position(0, 3, 1);

        Light light3 = Light("directional");
        light3.set_intensity(0.2);
        auto rotation1 = compute_rotation_matrix({ -20, 0, 0 });
        auto temp = rotation1 * Vector3D(0, 1, 0);
        light3.set_direction(temp[0], temp[1], temp[2]);

        std::vector<Light>* const lights = new std::vector<Light>();
        lights->push_back(light1);
        lights->push_back(light2);
        lights->push_back(light3);

        // Spheres
        Sphere sphere1 = Sphere(1.2);
        sphere1.set_cords(0, -1, 0);
        sphere1.set_rgb(255, 0, 0);
        sphere1.set_specular(500);
        sphere1.set_reflective(0.3);

        Sphere sphere2 = Sphere(1.2);
        sphere2.set_cords(-2.2, 0, -2);
        sphere2.set_rgb(0, 255, 0);
        sphere2.set_specular(10);
        sphere2.set_reflective(0.4);

        Sphere sphere3 = Sphere(1.2);
        sphere3.set_cords(2.2, 0, -2);
        sphere3.set_rgb(0, 0, 255);
        sphere3.set_specular(500);
        sphere3.set_reflective(0.3);

        std::vector<Sphere>* const spheres = new std::vector<Sphere>();
        spheres->push_back(sphere1);
        spheres->push_back(sphere2);
        spheres->push_back(sphere3);

        const int16_t cw = CW / 2;
        const int16_t ch = CH / 2;

        //Vector3D<double> O = { 0, 0, 0 };
        Vector3D<double> O = { 1.5, 3, -4 };
        const double recursion_depth = 3;
        const Vector3D<Vector3D<double>> rotation = compute_rotation_matrix({ 30, -30, 0 });
        //const Vector3D<Vector3D<double>> rotation = compute_rotation_matrix({ 0, 0, 0 });

        return RunDeps{ cw, ch,
                        O, recursion_depth, rotation,
                        mod, spheres, lights };
    }

    void thread_run(int16_t x1, int16_t x2, const RunDeps& deps)
    {
        uint32_t index = 0;
        for (int16_t x = x2; x < x1; ++x)
        {
            for (int16_t y = deps.ch; y > -deps.ch; --y)
            {
                const Vector3D<double> D = deps.rotation * canvas_to_viewport(x, y);
                const Vector3D<double> color = trace_ray(deps.O, D,
                    distance, MAX_RENDER_DISTANCE, deps.recursion_depth,
                    deps.model, deps.spheres, deps.lights);

                index = (y + deps.ch) * CH + (x - deps.cw);
                deps.pixels[index * 4 + 0] = color[0];
                deps.pixels[index * 4 + 1] = color[1];
                deps.pixels[index * 4 + 2] = color[2];
                deps.pixels[index * 4 + 3] = 255;
            }
        }
    }

    void run(const RunDeps& deps)
    {
        int16_t left = deps.cw;
        int16_t rigth = left - (CW / NUM_THREADS);
        std::vector<std::thread> threads = { };
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            threads.push_back(std::thread(thread_run, left, rigth, deps));
            left = rigth;
            rigth -= (CW / NUM_THREADS);
        }

        for (std::thread& t : threads)
            t.join();
    }
} // namespace Scene