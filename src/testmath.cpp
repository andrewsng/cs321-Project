#include "math3d.h"
#include <iostream>


void printVec4(const Vec4 v)
{
    for (int i = 0; i < 4; ++i)
        std::cout << v[i] << " ";
}


void printMat4(const Mat4 m)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            std::cout << m[i][j] << " ";
        }
        std::cout << "\n";
    }
}


int main()
{
    Vec4 nums = {
        1, 2, 3, 4
    };

    Mat4 id = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    };
    
    Mat4 a = {
        { 1, 2, 3, 4 },
        { 1, 2, 3, 4 },
        { 1, 2, 3, 4 },
        { 1, 2, 3, 4 }
    };
    
    printVec4(nums);
    std::cout << "\n\n";

    printMat4(id);
    std::cout << "\n\n";
    
    printVec4(id * nums);
    std::cout << "\n\n";

    printVec4(a * nums);
    std::cout << "\n\n";
    
    printMat4(id * id);
    std::cout << "\n\n";
    
    printMat4(a * id);
    std::cout << "\n\n";
    
    printMat4(id * a);
    std::cout << "\n\n";

    printMat4(a * a);
    std::cout << "\n\n";

    printVec4(Vec4{});
    std::cout << "\n\n";

    printMat4(Mat4{});
    std::cout << "\n\n";

    return 0;
}