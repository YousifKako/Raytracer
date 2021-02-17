#include <Objects/Object3D.hpp>

class Sphere : public Object3D
{
private:
    const uint64_t radius = 1;
    const Vector3D<int32_t> center = { 1, 1, 1 };

public:
    Sphere() { }
    Sphere(const uint32_t radius) : radius(radius) { }
    ~Sphere();
    const uint64_t get_radius() const;
};