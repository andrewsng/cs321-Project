// math3d.cpp
// Andrew S. Ng
// Created: 2021-04-19
// 
// For CS 321 DOS VGA Graphics
// Source for 3D math constructs

#include "math3d.h"


Mat4 appendRotationX(const Mat4 &transform, float angle)
{
    auto cosValue = std::cos(angle);
    auto sinValue = std::sin(angle);

    Mat4 result{transform};

    result[1][0] = cosValue*transform[1][0] + -sinValue*transform[2][0];
    result[1][1] = cosValue*transform[1][1] + -sinValue*transform[2][1];
    result[1][2] = cosValue*transform[1][2] + -sinValue*transform[2][2];
    result[2][0] = sinValue*transform[1][0] +  cosValue*transform[2][0];
    result[2][1] = sinValue*transform[1][1] +  cosValue*transform[2][1];
    result[2][2] = sinValue*transform[1][2] +  cosValue*transform[2][2];
    
    return result;
}


Mat4 appendRotationY(const Mat4 &transform, float angle)
{
    auto cosValue = std::cos(angle);
    auto sinValue = std::sin(angle);

    Mat4 result{transform};

    result[0][0] =  cosValue*transform[0][0] +  sinValue*transform[2][0];
    result[0][1] =  cosValue*transform[0][1] +  sinValue*transform[2][1];
    result[0][2] =  cosValue*transform[0][2] +  sinValue*transform[2][2];
    result[2][0] = -sinValue*transform[0][0] +  cosValue*transform[2][0];
    result[2][1] = -sinValue*transform[0][1] +  cosValue*transform[2][1];
    result[2][2] = -sinValue*transform[0][2] +  cosValue*transform[2][2];
    
    return result;
}


Mat4 appendRotationZ(const Mat4 &transform, float angle)
{
    auto cosValue = std::cos(angle);
    auto sinValue = std::sin(angle);

    Mat4 result{transform};

    result[0][0] = cosValue*transform[0][0] + -sinValue*transform[1][0];
    result[0][1] = cosValue*transform[0][1] + -sinValue*transform[1][1];
    result[0][2] = cosValue*transform[0][2] + -sinValue*transform[1][2];
    result[1][0] = sinValue*transform[0][0] +  cosValue*transform[1][0];
    result[1][1] = sinValue*transform[0][1] +  cosValue*transform[1][1];
    result[1][2] = sinValue*transform[0][2] +  cosValue*transform[1][2];
    
    return result;
}

