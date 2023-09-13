/// @ref core
/// @file matrix_utils.h
///
/// @defgroup CS
///
/// @brief The matrix_utils, which implements exactly and only the GLSL specification to the degree possible.
///

#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <math.h>

namespace glmCS
{
    // 定义一个 4x4 的矩阵结构
    typedef struct
    {
        float mat3[3][3];
        float mat4[4][4];
    } Matrix;

    // -------------------------------------------------------------------
    // 初始化单位矩阵
    void initIdentityMatrix4x4(Matrix *matrix)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matrix->mat4[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    // 执行平移变换
    void translateMatrix(Matrix *matrix, float x, float y, float z)
    {
        matrix->mat4[0][3] += x;
        matrix->mat4[1][3] += y;
        matrix->mat4[2][3] += z;
    }

    // 4x4矩阵乘法
    void matrixMultiply(Matrix *res, Matrix *a, float b[4][4])
    {
        int i, j, k;
        float temp[4][4];

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                temp[i][j] = 0.0f;
                for (k = 0; k < 4; k++)
                {
                    temp[i][j] += a->mat4[i][k] * b[k][j];
                }
            }
        }
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                res->mat4[i][j] = temp[i][j];
            }
        }
    }

    /// @brief 绕x/y/z轴旋转,旋转轴x,y,z有且只有一个为1
    /// @param angle 指定旋转角度
    /// @param matrix 输入与输出的矩阵
    /// @param x 旋转轴为x轴
    /// @param y 旋转轴为y轴
    /// @param z 旋转轴为z轴
    /// @return 
    int rotate(float angle, Matrix *matrix, bool x, bool y, bool z)
    {
        // 不旋转
        if ((int)x + (int)y + (int)z != 1)
        {
            fprintf(stderr, " [rotate error] Not rotated, make sure that the specified axis of rotation x,y,z has and only one is 1!\n");
            if((int)x + (int)y + (int)z == 0)
            {
                return 0;
            }
            return -1;
        }
        float radian = angle * M_PI / 180.0f;
        if (x == 1)
        {
            float rotationMatrix[4][4] = {
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, cosf(radian), sinf(radian), 0.0f},
                {0.0f, -sinf(radian), cosf(radian), 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            matrixMultiply(matrix, matrix, rotationMatrix);
        }
        else if (y == 1)
        {
            float rotationMatrix[4][4] = {
                {cosf(radian), 0.0f, -sinf(radian), 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {sinf(radian), 0.0f, cosf(radian), 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            matrixMultiply(matrix, matrix, rotationMatrix);
        }
        else if (z == 1)
        {
            float rotationMatrix[4][4] = {
                {cosf(radian), sinf(radian), 0.0f, 0.0f},
                {-sinf(radian), cosf(radian), 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            matrixMultiply(matrix, matrix, rotationMatrix);
        }
        return 1;
    }

    // 缩放矩阵
    void scaleMatrix(Matrix *matrix, float x, float y, float z)
    {
        matrix->mat4[0][0] *= x;
        matrix->mat4[0][1] *= x;
        matrix->mat4[0][2] *= x;

        matrix->mat4[1][0] *= y;
        matrix->mat4[1][1] *= y;
        matrix->mat4[1][2] *= y;

        matrix->mat4[2][0] *= z;
        matrix->mat4[2][1] *= z;
        matrix->mat4[2][2] *= z;
    }

    // 打印矩阵
    void printMatrix(const Matrix *matrix)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%.2f\t", matrix->mat4[i][j]);
            }
            printf("\n");
        }
    }
} // namespace glmCS