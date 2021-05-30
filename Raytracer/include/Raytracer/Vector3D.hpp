#pragma once

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

    constexpr
    Vector3D<T> mult(const Vector3D<T>& vec) const
    {
        return Vector3D<T>((this->x1 * vec.x1),
                           (this->x2 * vec.x2),
                           (this->x3 * vec.x3));
    }

    constexpr
    double length() const
    {
        return sqrt(pow(this->x1, 2) +
                    pow(this->x2, 2) +
                    pow(this->x3, 2));
    }

    const
    T& operator[](const uint16_t index) const
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

    constexpr
    auto operator*(const Vector3D<T>& vec) const
    {
        return (this->x1 * vec[0]) +
               (this->x2 * vec[1]) +
               (this->x3 * vec[2]);
    }

    template <typename V>
    constexpr
    auto operator*(const Vector3D<V>& vec) const
    {
        return (this->x1 * vec[0]) +
               (this->x2 * vec[1]) +
               (this->x3 * vec[2]);
    }

    template <typename V>
    constexpr
    auto operator/(const V& i) const
    {
        return Vector3D<decltype(this->x1 * i)>
                                ((this->x1 / i),
                                 (this->x2 / i),
                                 (this->x3 / i));
    }

    constexpr
    auto operator+(const Vector3D<T>& vec) const
    {
        return Vector3D<decltype(this->x1 * vec[0])>
                                ((this->x1 + vec[0]),
                                 (this->x2 + vec[1]),
                                 (this->x3 + vec[2]));
    }

    template <typename V>
    constexpr
    auto operator+(const Vector3D<V>& vec) const
    {
        return Vector3D<decltype(this->x1 * vec[0])>
                                ((this->x1 + vec[0]),
                                 (this->x2 + vec[1]),
                                 (this->x3 + vec[2]));
    }

    template <typename V>
    constexpr
    auto operator-(const Vector3D<V>& vec) const
    {
        return Vector3D<decltype(this->x1 * vec[0])>
                                ((this->x1 - vec[0]),
                                 (this->x2 - vec[1]),
                                 (this->x3 - vec[2]));
    }

    constexpr
    auto operator-() const
    {
        return Vector3D<T>((-this->x1), (-this->x2), (-this->x3));
    }

    template <typename V, typename K>
    friend auto operator*(const Vector3D<V>& vec, const K& i);

    template <typename V, typename K>
    friend auto operator*(const K& i, const Vector3D<V>& vec);

    template <typename V, typename K>
    friend auto operator*(const Vector3D<Vector3D<V>>& vec, const Vector3D<K>& vec2);

    template <typename V, typename K>
    friend auto operator+(const Vector3D<V>& vec, const K& i);

    template <typename V, typename K>
    friend auto operator+(const K& i, const Vector3D<V>& vec);
};

template <typename V>
std::ostream& operator<<(std::ostream& os, const Vector3D<V>& vec)
{
    os << '<';
    os << vec[0] << ", ";
    os << vec[1] << ", ";
    os << vec[2] << '>';
    return os;
}

template <typename V, typename K>
auto operator*(const Vector3D<V>& vec, const K& i)
{
    return Vector3D<decltype(vec[0] * i)>
        (vec[0] * i, vec[1] * i, vec[2] * i);
}

template <typename V, typename K>
auto operator*(const K& i, const Vector3D<V>& vec)
{
    return vec * i;
}

template <typename V, typename K>
auto operator*(const Vector3D<Vector3D<V>>& vec, const Vector3D<K>& vec2)
{
    return Vector3D<decltype(vec[0][0] * vec2[0])>
        ((vec[0][0] * vec2[0]) + (vec[0][1] * vec2[1]) + (vec[0][2] * vec2[2]),
         (vec[1][0] * vec2[0]) + (vec[1][1] * vec2[1]) + (vec[1][2] * vec2[2]),
         (vec[2][0] * vec2[0]) + (vec[2][1] * vec2[1]) + (vec[2][2] * vec2[2]));
}

template <typename V, typename K>
auto operator+(const Vector3D<V>& vec, const K& i)
{
    return Vector3D<decltype(vec[0] * i)>
        (vec[0] + i, vec[1] + i, vec[2] + i);
}

template <typename V, typename K>
auto operator+(const K& i, const Vector3D<V>& vec)
{
    return vec + i;
}

template <typename V, typename K>
bool operator==(const Vector3D<V>& vec, const Vector3D<K>& vec2)
{
    if (vec[0] == vec2[0] && vec[1] == vec2[1] && vec[2] == vec2[2])
        return true;
    return false;
}