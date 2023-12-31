/// @ref core
/// @file csmatrix_utils.h
///
/// @defgroup CS https://github.com/CSsaan/OpenGL_GLSL
///
/// @brief The csmatrix_utils, which implements exactly and only the GLSL specification to the degree possible.
/// The csmatrix_utils is a utility library written in C++ language, aiming to provide accurate and exclusive implementation of GLSL matrix operations to the best extent possible.
///
/// glmCS::Matrix<float, 4, 4> mat4x4 = glmCS::initIdentityMatrix<float, 4>();
/// mat4x4 = glmCS::scaleMatrix<float>(mat4x4, 1, 2, 3);
/// mat4x4 = glmCS::translateMatrix<float>(mat4x4, 1, 2, 3);
/// mat4x4 = glmCS::rotate(270, mat4x4, 1, 0, 0);
/// glmCS::printMatrix<float, 4, 4>(&mat4x4);
///

#ifndef __CSMATRIX_UTILS_H__
#define __CSMATRIX_UTILS_H__

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef unsigned char glmcs_uc;
typedef unsigned short glmcs_us;

enum
{
    GLMCS_false = -1,
    GLMCS_not_rotate = 0,
    GLMCS_ok = 1
};

namespace glmCS
{
    // 定义一个 MxN 的二维矩阵结构
    template <typename T, size_t rows, size_t cols>
    struct Matrix
    {
        T mat[rows][cols];
    };

    // 定义一个 vec3 的向量
    typedef struct Vector3
    {
        float x, y, z;
    } Vector3;

    // -------------------------------------------------------------------
    // 初始化单位矩阵
    template <typename T, size_t N>
    inline Matrix<T, N, N> initIdentityMatrix()
    {
        Matrix<T, N, N> matrix;
        size_t i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                matrix.mat[i][j] = (i == j) ? T(1) : T(0);
            }
        }
        return matrix;
    }

    // 初始化vec3向量
    inline Vector3 vec3(float x, float y, float z)
    {
        Vector3 v;
        v.x = x;
        v.y = y;
        v.z = z;
        return v;
    }

    // 获取vec3单位向量
    static int normalize(Vector3 *v)
    {
        float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
        if (0 == length)
        {
            printf("[%s:%i]lookAt()::normalize() ZeroDivisionError: division by zero!\n", __FILE__, __LINE__);
            return GLMCS_false;
        }
        v->x /= length;
        v->y /= length;
        v->z /= length;
        return GLMCS_ok;
    }

    // 获取vec3叉乘向量
    static void cross(Vector3 *result, const Vector3 *v1, const Vector3 *v2)
    {
        result->x = v1->y * v2->z - v1->z * v2->y;
        result->y = v1->z * v2->x - v1->x * v2->z;
        result->z = v1->x * v2->y - v1->y * v2->x;
    }

    // 获取vec3向量内积
    static float dot(const Vector3 *v1, const Vector3 *v2)
    {
        return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
    }

    // 获取vec3相减向量
    static void subtract(Vector3 *result, const Vector3 *v1, const Vector3 *v2)
    {
        result->x = v1->x - v2->x;
        result->y = v1->y - v2->y;
        result->z = v1->z - v2->z;
    }

    /// @brief 创建一个观察矩阵，用于将眼睛位置转换为观察坐标系
    /// @param eye 相机位置的三维向量
    /// @param target 目标位置的三维向量
    /// @param up 上方向的三维向量
    /// @return 返回一个观察矩阵
    inline Matrix<float, 4, 4> lookAt(Vector3 eye, Vector3 target, Vector3 up)
    {
        Vector3 forward, right, upVec;

        subtract(&forward, &target, &eye);
        normalize(&forward);

        cross(&right, &forward, &up);
        normalize(&right);

        cross(&upVec, &right, &forward);
        normalize(&upVec);

        Matrix<float, 4, 4> Matrix4;

        Matrix4.mat[0][0] = right.x;
        Matrix4.mat[0][1] = upVec.x;
        Matrix4.mat[0][2] = -forward.x;
        Matrix4.mat[0][3] = 0.0f;

        Matrix4.mat[1][0] = right.y;
        Matrix4.mat[1][1] = upVec.y;
        Matrix4.mat[1][2] = -forward.y;
        Matrix4.mat[1][3] = 0.0f;

        Matrix4.mat[2][0] = right.z;
        Matrix4.mat[2][1] = upVec.z;
        Matrix4.mat[2][2] = -forward.z;
        Matrix4.mat[2][3] = 0.0f;

        Matrix4.mat[3][0] = -dot(&right, &eye);
        Matrix4.mat[3][1] = -dot(&upVec, &eye);
        Matrix4.mat[3][2] = dot(&forward, &eye);
        Matrix4.mat[3][3] = 1.0f;
        return Matrix4;
    }

    /// @brief 生成透视投影矩阵
    /// @param fov 视野角度
    /// @param aspectRatio 纵横比
    /// @param nearPlane 近平面距离
    /// @param farPlane 远平面距离
    /// @return 生成的透视投影矩阵
    inline Matrix<float, 4, 4> perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        Matrix<float, 4, 4> Matrix4;
        float f = 1.0f / tanf(fov * 0.5f);

        Matrix4.mat[0][0] = f / aspectRatio;
        Matrix4.mat[0][1] = 0.0f;
        Matrix4.mat[0][2] = 0.0f;
        Matrix4.mat[0][3] = 0.0f;

        Matrix4.mat[1][0] = 0.0f;
        Matrix4.mat[1][1] = f;
        Matrix4.mat[1][2] = 0.0f;
        Matrix4.mat[1][3] = 0.0f;

        Matrix4.mat[2][0] = 0.0f;
        Matrix4.mat[2][1] = 0.0f;
        Matrix4.mat[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
        Matrix4.mat[2][3] = -1.0f;

        Matrix4.mat[3][0] = 0.0f;
        Matrix4.mat[3][1] = 0.0f;
        Matrix4.mat[3][2] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
        Matrix4.mat[3][3] = 0.0f;
        return Matrix4;
    }

    /// @brief 在给定的矩阵上进行平移变换
    /// @param matrix 要进行平移变换的矩阵
    /// @param x X轴方向上的平移量
    /// @param y Y轴方向上的平移量
    /// @param z Z轴方向上的平移量
    template <typename T>
    inline Matrix<T, 4, 4> translateMatrix(const Matrix<T, 4, 4> &matrix, T x, T y, T z)
    {
        Matrix<T, 4, 4> result = matrix;
        result.mat[3][0] += x;
        result.mat[3][1] += y;
        result.mat[3][2] += z;
        return result;
    }

    // 4x4矩阵乘法
    template <typename T, size_t rows, size_t cols>
    Matrix<T, rows, cols> matrixMultiply(const Matrix<T, rows, cols> &matrix, const T b[cols][cols])
    {
        Matrix<T, rows, cols> result;
        T temp[rows][cols];
        size_t i, j, k;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                temp[i][j] = T(0);
                for (k = 0; k < cols; k++)
                {
                    temp[i][j] += matrix.mat[i][k] * b[k][j];
                }
            }
        }
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                result.mat[i][j] = temp[i][j];
            }
        }
        return result;
    }

    /// @brief 绕x/y/z轴旋转,旋转轴x,y,z有且只有一个为1
    /// @param angle 指定旋转角度
    /// @param matrix 输入与输出的矩阵
    /// @param x 旋转轴为x轴
    /// @param y 旋转轴为y轴
    /// @param z 旋转轴为z轴
    /// @return
    inline Matrix<float, 4, 4> rotate(float angle, const Matrix<float, 4, 4> &matrix, bool x, bool y, bool z)
    {
        Matrix<float, 4, 4> result = matrix;
        // 不旋转
        if ((int)x + (int)y + (int)z != 1)
        {
            fprintf(stderr, "[%s:%i] [rotate error] Not rotated, make sure that the specified axis of rotation x,y,z has and only one is 1!\n", __FILE__, __LINE__);
            if ((int)x + (int)y + (int)z == 0)
            {
                return result; // 返回未旋转的矩阵
            }
            return result; // 返回原始矩阵
        }
        float radian = angle * M_PI / 180.0f;
        if (x == 1)
        {
            float rotationMatrix[4][4] = {
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, cosf(radian), sinf(radian), 0.0f},
                {0.0f, -sinf(radian), cosf(radian), 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            result = matrixMultiply(matrix, rotationMatrix);
        }
        else if (y == 1)
        {
            float rotationMatrix[4][4] = {
                {cosf(radian), 0.0f, -sinf(radian), 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {sinf(radian), 0.0f, cosf(radian), 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            result = matrixMultiply(matrix, rotationMatrix);
        }
        else if (z == 1)
        {
            float rotationMatrix[4][4] = {
                {cosf(radian), sinf(radian), 0.0f, 0.0f},
                {-sinf(radian), cosf(radian), 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}};
            result = matrixMultiply(matrix, rotationMatrix);
        }
        return result;
    }

    /// @brief 在给定的矩阵上进行缩放变换
    /// @param matrix 要进行缩放变换的矩阵
    /// @param x X轴方向上的缩放因子
    /// @param y Y轴方向上的缩放因子
    /// @param z Z轴方向上的缩放因子
    template <typename T>
    inline Matrix<T, 4, 4> scaleMatrix(const Matrix<T, 4, 4> &matrix, T x, T y, T z)
    {
        Matrix<T, 4, 4> result = matrix;
        result.mat[0][0] *= x;
        result.mat[0][1] *= x;
        result.mat[0][2] *= x;

        result.mat[1][0] *= y;
        result.mat[1][1] *= y;
        result.mat[1][2] *= y;

        result.mat[2][0] *= z;
        result.mat[2][1] *= z;
        result.mat[2][2] *= z;
        return result;
    }

    // 打印矩阵
    template <typename T, size_t rows, size_t cols>
    inline void printMatrix(const Matrix<T, rows, cols> *matrix)
    {
        size_t i, j;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                printf("%.2f\t", matrix->mat[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
} // namespace glmCS

#endif // __CSMATRIX_UTILS_H__
