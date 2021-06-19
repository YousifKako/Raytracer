#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <windows.h>

#include <Objects/Light.hpp>
#include <Objects/Triangle.hpp>
#include <Utilities.hpp>

namespace TriangleRayTrace {
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

    const std::pair<bool, const Triangle*>*
    closest_intersection(const Vector3D<double>&             O,
                         const Vector3D<double>&             D,
                         const double&                   t_min,
                         const double&                   t_max,
                         Vector3D<double>&              coords,
                         const std::vector<Mesh*>* const model)
    {
        double closest_t = MAX_RENDER_DISTANCE;
        Triangle* closest_triangle = new Triangle();
        bool intersect = false;

        for (Mesh* const mesh : *model)
        {
            for (const Triangle triangle : *(mesh->triangles))
            {
                const auto V = triangle.get_vertices();
                const auto N = triangle.get_normals();

                if (moller_trumbore(coords, O, D, V))
                {
                    if (closest_t >= t_min && closest_t <= t_max && coords[0] < closest_t)
                    {
                        closest_t = coords[0];
                        *closest_triangle = triangle;
                        intersect = true;
                    }
                }
            }
        }

        return new std::pair<bool, const Triangle*>(intersect, closest_triangle);
    }

    const Vector3D<double>
    get_color(const Vector3D<double>& baryc, const Triangle* const triangle)
    {
        auto u = baryc[1];
        auto v = baryc[2];
        auto w = 1 - u - v;

        auto c1 = triangle->get_vertices_rgb()[0];
        auto c2 = triangle->get_vertices_rgb()[1];
        auto c3 = triangle->get_vertices_rgb()[2];

        return (w * c1) * 255 + (u * c2) * 255 + (v * c3) * 255;
    }

    const double
    compute_lighting(const Vector3D<double>& P,
                     const Vector3D<double>& N,
                     const Vector3D<double>& V,
                     const double& s,
                     const std::vector<Light*>* const lights,
                     const std::vector<Mesh*>* const model)
    {
        double               i = 0.f;
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
                    L = light->get_cords() - P;
                    t_max = 1;
                }
                else
                {
                    L = light->get_cords();
                    t_max = MAX_RENDER_DISTANCE;
                }

                // Shadows
                Vector3D<double> coords = Vector3D<double>();
                const auto shadow = closest_intersection(P, L, EPSILON, t_max, coords, model);
                if (shadow->first)
                    continue;

                // Diffuse Reflection
                const double n_dot_l = N * L;
                if (n_dot_l > 0)
                    i += light->get_intensity() * (n_dot_l / (N.length() * L.length()));

                // Specular Reflection
                if (s != -1)
                {
                    const Vector3D<double> R = reflect_ray(L, N);
                    const double r_dot_v = R * V;
                    if (r_dot_v > 0)
                        i += light->get_intensity() * std::pow(r_dot_v / (R.length() * V.length()), s);
                }
            }
        }

        return i;
    }

    inline const Vector3D<double>
    trace_ray(const Vector3D<double>&               O,
              const Vector3D<double>&               D,
              const double&                     t_min,
              const double&                     t_max,
              const double&           recursion_depth,
              const std::vector<Light*>* const lights,
              const std::vector<Mesh*>*  const  model)
    {
        // Calculate Barycentric Coordinate's using Moller Trumbore Algorithm
        Vector3D<double> coords = Vector3D<double>();
        auto pair = closest_intersection(O, D, t_min, t_max, coords, model);
        if (!pair->first)
            return BACKGROUND_COLOR;
        const Triangle* const closest_triangle = pair->second;

        // Get RBG colors
        auto pixel = get_color(coords, closest_triangle);

        const auto P = O + coords[0] * D;
        const Vector3D<double> local_color = pixel *
            compute_lighting(P, closest_triangle->get_normals(), -D, closest_triangle->get_specular(), lights, model);
        
        const double r = closest_triangle->get_reflective();
        if (recursion_depth <= 0 || r <= 0)
        {
            delete pair, closest_triangle;
            return local_color;
        }
        
        const Vector3D<double> R = reflect_ray(-D, closest_triangle->get_normals());
        const Vector3D<double> reflected_color = trace_ray(P, R, EPSILON, t_max, recursion_depth - 1, lights, model);

        Vector3D<double> color = local_color * (1 - r) + reflected_color * r;
        delete pair, closest_triangle;

        return color;
    }

    namespace Scene {
        typedef struct RunDeps
        {
            const std::vector<Light*>* const lights;
            int16_t cw = 0;
            int16_t ch = 0;
            HDC hdc = { };
            HDC memdc = { };
            Vector3D<double> O = { };
            double recursion_depth = 0;
            Vector3D<Vector3D<double>> rotation = { };
            const std::vector<Mesh*>* const model;
        } RunDeps;

        const RunDeps setup()
        {
            Model* model = new Model("..\\assets\\Test.obj");
            model->load();
            const std::vector<Mesh*>* const mod = model->get();

            // Lights
            Light* const light1 = new Light("ambient");
            light1->set_intensity(0.5);

            Light* const light2 = new Light("point");
            light2->set_intensity(0.2);
            light2->set_position(0, 3, 1);

            Light* const light3 = new Light("directional");
            light3->set_intensity(0.2);
            auto rotation1 = compute_rotation_matrix({ -20, 0, 0 });
            auto temp = rotation1 * Vector3D(0, 1, 0);
            light3->set_direction(temp[0], temp[1], temp[2]);

            std::vector<Light*>* const lights = new std::vector<Light*>();
            lights->push_back(light1);
            lights->push_back(light2);
            lights->push_back(light3);

            const int16_t cw = CW / 2;
            const int16_t ch = CH / 2;
             
            //const HDC hdc = GetDC(NULL);
            const HDC hdc = GetDC(GetConsoleWindow());
            const HDC memdc = CreateCompatibleDC(hdc);
            const HBITMAP hbitmap = CreateCompatibleBitmap(hdc, CW, CH);
            SelectObject(memdc, hbitmap);

            //Vector3D<double> O = { -2, 1, 0 };
            Vector3D<double> O = { 0, 1, -1 };
            const double recursion_depth = 1;
            const Vector3D<Vector3D<double>> rotation = compute_rotation_matrix({ 15, -15, 0 });
            //const Vector3D<Vector3D<double>> rotation = compute_rotation_matrix({ 30, 30, 0 });

            return RunDeps{ lights,
                            cw, ch,
                            hdc, memdc,
                            O, recursion_depth, rotation, mod };
        }
        void run(const RunDeps& deps)
        {
            for (int32_t x = -deps.cw; x < deps.cw; ++x)
            {
                for (int32_t y = deps.ch; y > -deps.ch; --y)
                {
                    const Vector3D<double> D = deps.rotation * canvas_to_viewport(x, y);
                    const Vector3D<double> color = TriangleRayTrace::trace_ray(deps.O, D, 
                        distance, MAX_RENDER_DISTANCE, deps.recursion_depth,
                        deps.lights, deps.model);
                    SetPixel(deps.memdc, x + deps.ch, -y + deps.cw, RGB(color[0], color[1], color[2]));
                }
            }
            BitBlt(deps.hdc, 0, 80, CW, CH, deps.memdc, 0, 0, SRCCOPY);
        }

    } // namespace Scene

} // namespace TriangleRayTrace