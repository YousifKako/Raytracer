#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <windows.h>

#include <Objects/Light.hpp>
#include <Objects/Triangle.hpp>
#include <Utilities.hpp>

namespace TriangleRayTrace {
    const Vector3D<double>
    moller_trumbore(const Vector3D<Vector3D<double>>& V,
                    const Vector3D<double>&           O,
                    const Vector3D<double>&           D)
    {
        auto E1 = V[1] - V[0];
        auto E2 = V[2] - V[0];
        auto T = O - V[0];

        auto P = D.cross(E2);
        auto Q = T.cross(E1);

        auto det_inv = 1.f / (P * E1);

        return Vector3D<double>(Q * E2, P * T, Q * D) * det_inv;
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

    const Vector3D<double>*
    trace_ray(const Vector3D<double>& O,
              const Vector3D<double>& D,
              const std::vector<Triangle*>* triangles)
    {
        for (Triangle* const triangle : *triangles)
        {
            std::tuple<double, double, double> bc = { 0.f, 0.f, 0.f };

            const auto V = triangle->get_vertices();
            //const auto N = triangle->get_normals();

            const auto v0v1 = V[1] - V[0];
            const auto v0v2 = V[2] - V[0];
            // no need to normalize
            const auto N = v0v1.cross(v0v2); // N

            // Check if Ray and Plane (Triangle) are parallel
            const double N_dot_D = N * D;
            if (N_dot_D < EPSILON) // Check
                return nullptr;

            // Calculate Barycentric Coordinate's using Moller Trumbore Algorithm
            auto baryc = moller_trumbore(V, O, D);

            // Check if a Point on the Plain is within the Triangle
            if (baryc[0]            < EPSILON ||
                baryc[1]            > 1       || baryc[1] < 0 ||
                baryc[1] + baryc[2] > 1       ||
                baryc[2]            > 1       || baryc[2] < 0)
                return nullptr;

            // Get RBG colors
            auto pixel = get_color(baryc, triangle);

            return new Vector3D<double>(pixel[0], pixel[1], pixel[2]);
        }

        return nullptr;
    }

    namespace Scene {
        typedef struct RunDeps
        {
            std::vector<Triangle*>* const triangles = nullptr;
            int16_t cw = 0;
            int16_t ch = 0;
            HDC hdc = { };
            HDC memdc = { };
            Vector3D<double> O = { };
            double recursion_depth = 0;
            Vector3D<Vector3D<double>> rotation = { };
        } RunDeps;

        const RunDeps setup()
        {
            Triangle* const triangle1 = new Triangle();
            triangle1->set_vertices({ -1, -1, 5 }, { 1, -1, 5 }, { 0, 1, 5 });
            triangle1->set_vertices_rgb({ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 });

            std::vector<Triangle*>* const triangles = new std::vector<Triangle*>();
            triangles->push_back(triangle1);

            const int16_t cw = CW / 2;
            const int16_t ch = CH / 2;

            //const HDC hdc = GetDC(NULL);
            const HDC hdc = GetDC(GetConsoleWindow());
            const HDC memdc = CreateCompatibleDC(hdc);
            const HBITMAP hbitmap = CreateCompatibleBitmap(hdc, CW, CH);
            SelectObject(memdc, hbitmap);

            Vector3D<double> O = { 0, 0, 0 };
            const double recursion_depth = 3;
            const Vector3D<Vector3D<double>> rotation = compute_rotation_matrix({ 0, 0, 0 });

            return RunDeps{ triangles,
                            cw, ch,
                            hdc, memdc,
                            O, recursion_depth, rotation };
        }
        void run(const RunDeps& deps)
        {
            for (int32_t x = -deps.cw; x < deps.cw; ++x)
            {
                for (int32_t y = deps.ch; y > -deps.ch; --y)
                {
                    const Vector3D<double> D = deps.rotation * canvas_to_viewport(x, y);
                    const Vector3D<double>* color = TriangleRayTrace::trace_ray(deps.O, D, deps.triangles);
                    if (color == nullptr)
                        SetPixel(deps.memdc, x + deps.ch, -y + deps.cw, RGB(0, 0, 0));
                    else
                        SetPixel(deps.memdc, x + deps.ch, -y + deps.cw, RGB((*color)[0], (*color)[1], (*color)[2]));
                }
            }
            BitBlt(deps.hdc, 0, 40, CW, CH, deps.memdc, 0, 0, SRCCOPY);
        }

    } // namespace Scene

} // namespace TriangleRayTrace