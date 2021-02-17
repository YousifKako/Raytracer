#include <iostream>
#include <string>

template <typename T>
class Vector3D
{
private:
    T x1 = nullptr;
    T x2 = nullptr;
    T x3 = nullptr;

    void change_vars(const T x1, const T x2, const T x3)
    {
        this->x1 = x1;
        this->x2 = x2;
        this->x3 = x3;
    }

public:
    Vector3D() : x1(0), x2(0), x3(0) { }
    Vector3D(const T x1, const T x2, const T x3) : x1(x1), x2(x2), x3(x3) { }
    ~Vector3D() { }
    const Vector3D<T> mult(const Vector3D<T>& vec) const
    {
        return Vector3D<T>((this->x1 * vec.x1),
                           (this->x2 * vec.x2),
                           (this->x3 * vec.x3));
    }

    const T& operator[](const uint16_t index) const
    {
        switch (index)
        {
        case 0: return x1;
        case 1: return x2;
        case 2: return x3;
        }

        std::cerr << "Index [" << std::to_string(index) << "] out of bounds" << std::endl;
        throw EXIT_FAILURE;
    }

    template <typename V>
    const int64_t operator*(const Vector3D<V>& vec) const
    {
        return (int64_t(this->x1) * int64_t(vec[0])) +
               (int64_t(this->x2) * int64_t(vec[1])) +
               (int64_t(this->x3) * int64_t(vec[2]));
    }

    template <typename V>
    const Vector3D<V> operator-(const Vector3D<V>& vec) const
    {
        return Vector3D<V>((this->x1 - vec[0]),
                           (this->x2 - vec[1]),
                           (this->x3 - vec[2]));
    }

    template <typename V, typename K>
    friend Vector3D<V>& operator*(Vector3D<V>& vec, const K i);

    template <typename V, typename K>
    friend Vector3D<V>& operator*(const K i, Vector3D<V>& vec);

    template <typename V, typename K>
    friend Vector3D<V>& operator+(const Vector3D<V>& vec, const K i);

    template <typename V, typename K>
    friend Vector3D<V>& operator+(const K i, const Vector3D<V>& vec);
};

template <typename V>
std::ostream& operator<<(std::ostream& os, const Vector3D<V>& vec)
{
    os << vec[0] << '\n';
    os << vec[1] << '\n';
    os << vec[2] << '\n';
    return os;
}

template <typename V, typename K>
Vector3D<V>& operator*(Vector3D<V>& vec, const K i)
{
    vec.change_vars(vec[0] * i, vec[1] * i, vec[2] * i);
    return vec;
}

template <typename V, typename K>
Vector3D<V>& operator*(const K i, Vector3D<V>& vec)
{
    vec.change_vars(vec[0] * i, vec[1] * i, vec[2] * i);
    return vec;
}

template <typename V, typename K>
Vector3D<V>& operator+(const Vector3D<V>& vec, const K i)
{
    vec.change_vars(vec[0] + i, vec[1] + i, vec[2] + i);
    return vec;
}

template <typename V, typename K>
Vector3D<V>& operator+(const K i, const Vector3D<V>& vec)
{
    vec.change_vars(vec[0] + i, vec[1] + i, vec[2] + i);
    return vec;
}

template <typename V, typename K>
bool operator==(const Vector3D<V>& vec, const Vector3D<K>& vec2)
{
    if (vec[0] == vec2[0] && vec[1] == vec2[1] && vec[2] == vec2[2])
        return true;
    return false;
}