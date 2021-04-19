// math3d.h
// Andrew S. Ng
// Created: 2021-04-11
// 
// For CS 321 DOS VGA Graphics
// Header for 3D math constructs

#ifndef MATH3D_H
#define MATH3D_H

#include <array>
#include <algorithm>
#include <initializer_list>
#include <cassert>
#include <cmath>


class Vec4
{

public:
    
    Vec4() = default;
    
    Vec4(std::initializer_list<float> list)
    {
        assert(list.size() == 4);
        std::copy(list.begin(), list.end(), _data.begin());
    }
    
    float &operator[](std::size_t i) { return _data[i]; }
    const float &operator[](std::size_t i) const { return _data[i]; }

private:

    std::array<float, 4> _data;

};


class Mat4
{

public:
    
    Mat4() = default;
    
    Mat4(std::initializer_list<std::array<float, 4>> list)
    {
        assert(list.size() == 4);
        std::copy(list.begin(), list.end(), _data.begin());
    }

    std::array<float, 4> &operator[](std::size_t i) { return _data[i]; }
    const std::array<float, 4> &operator[](std::size_t i) const { return _data[i]; }

private:

    std::array<std::array<float, 4>, 4> _data;

};


inline Vec4 operator*(const Mat4 &lhs, const Vec4 &rhs)
{
    Vec4 result{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i] += lhs[i][j] * rhs[j];
        }
    }
    return result;
}


inline Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs)
{
    Mat4 result{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return result;
}


Mat4 appendRotationX(const Mat4 &transform, float angle);

Mat4 appendRotationY(const Mat4 &transform, float angle);

Mat4 appendRotationZ(const Mat4 &transform, float angle);


#endif  // MATH3D_H