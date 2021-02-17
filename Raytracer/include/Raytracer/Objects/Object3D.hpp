#include <Vector3D.hpp>

class Object3D
{
private:
    Vector3D<int32_t>*  cords = nullptr;
    Vector3D<uint16_t>* rgb   = nullptr;

public:
    Object3D() { }
    ~Object3D();
    void set_cords(const int32_t x, const int32_t y, const int32_t z);
    const Vector3D<int32_t>& get_cords() const;
    void set_rgb(const uint16_t r, const uint16_t g, const uint16_t b);
    const Vector3D<uint16_t>& get_rgb() const;
};