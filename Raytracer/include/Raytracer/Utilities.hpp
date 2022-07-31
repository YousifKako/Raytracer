#pragma once

#define BACKGROUND_COLOR    Vector3D<double>(200, 200, 200)
#define PI                  3.14159265358979323846
#define EPSILON             0.0001
#define MAX_RENDER_DISTANCE INFINITY

const int16_t CW      = 800;
const int16_t CH      = 800;
const int16_t VW      = 2;
const int16_t VH      = 2;
const double distance = 1;

inline const Vector3D<double>
canvas_to_viewport(const double x, const double y)
{
    return { x * VW / CW, y * VH / CH, distance };
}

const Vector3D<double>
reflect_ray(const Vector3D<double>& R, const Vector3D<double>& N)
{
    return 2 * N * (N * R) - R;
}

const Vector3D<Vector3D<double>>
compute_rotation_matrix(const Vector3D<double>& vec)
{
    const double x0 = vec[0] * PI / 180.f;
    const double x1 = vec[1] * PI / 180.f;
    const double x2 = vec[2] * PI / 180.f;

    const double sinX = std::sin(x0);
    const double sinY = std::sin(x1);
    const double sinZ = std::sin(x2);

    const double cosX = std::cos(x0);
    const double cosY = std::cos(x1);
    const double cosZ = std::cos(x2);

    return { {cosZ * cosY, cosZ * sinY * sinX - sinZ * cosX, cosZ * sinY * cosX + sinZ * sinX},
             {sinZ * cosY, sinZ * sinY * sinX + cosZ * cosX, sinZ * sinY * cosX - cosZ * sinX},
             {      -sinY,                      cosY * sinX,                      cosY * cosX} };
}