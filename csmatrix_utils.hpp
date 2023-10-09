/// @ref core
/// @file csmatrix_utils.h
///
/// @defgroup CS https://github.com/CSsaan/OpenGL_GLSL
///
/// @brief The csmatrix_utils, which implements exactly and only the GLSL specification to the degree possible.
///
/// glmCS::initIdentityMatrix4x4(&modelMatrix);
/// glmCS::Matrix lookmat4 = glmCS::lookAt(glmCS::vec3(0.0f, 0.0f, 3.0f),glmCS::vec3(0.0f, 0.0f, 0.0f) ,glmCS::vec3(0.0f, 1.0f, 0.0f));
/// glmCS::Matrix persprctmat4 = glmCS::perspective(45.0f, 4.0f/3.0f, 0.01f, 100.0f);
/// glmCS::translateMatrix(&modelMatrix, 0.825f, y_step, 0.0f);
/// glmCS::scaleMatrix(&modelMatrix, 0.05f, 2.0f, 1.0f);
/// glmCS::rotate(270, &modelMatrix, 0, 0, 1);
/// glmCS::printMatrix(&modelMatrix);
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

#ifdef __cplusplus
extern "C"
{
#endif

    namespace glmCS
    {
        // 定义一个 4x4 的矩阵结构
        typedef struct
        {
            float mat3[3][3];
            float mat4[4][4];
        } Matrix;

        // 定义一个 vec3 的向量
        typedef struct
        {
            float x, y, z;
        } Vector3;

        // -------------------------------------------------------------------
        // 初始化单位矩阵
        inline void initIdentityMatrix4x4(Matrix *matrix)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    matrix->mat4[i][j] = (i == j) ? 1.0f : 0.0f;
                }
            }
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
        inline Matrix lookAt(Vector3 eye, Vector3 target, Vector3 up)
        {
            Vector3 forward, right, upVec;

            subtract(&forward, &target, &eye);
            normalize(&forward);

            cross(&right, &forward, &up);
            normalize(&right);

            cross(&upVec, &right, &forward);
            normalize(&upVec);

            Matrix Matrix4;

            Matrix4.mat4[0][0] = right.x;
            Matrix4.mat4[0][1] = upVec.x;
            Matrix4.mat4[0][2] = -forward.x;
            Matrix4.mat4[0][3] = 0.0f;

            Matrix4.mat4[1][0] = right.y;
            Matrix4.mat4[1][1] = upVec.y;
            Matrix4.mat4[1][2] = -forward.y;
            Matrix4.mat4[1][3] = 0.0f;

            Matrix4.mat4[2][0] = right.z;
            Matrix4.mat4[2][1] = upVec.z;
            Matrix4.mat4[2][2] = -forward.z;
            Matrix4.mat4[2][3] = 0.0f;

            Matrix4.mat4[3][0] = -dot(&right, &eye);
            Matrix4.mat4[3][1] = -dot(&upVec, &eye);
            Matrix4.mat4[3][2] = dot(&forward, &eye);
            Matrix4.mat4[3][3] = 1.0f;
            return Matrix4;
        }

        /// @brief 生成透视投影矩阵
        /// @param fov 视野角度
        /// @param aspectRatio 纵横比
        /// @param nearPlane 近平面距离
        /// @param farPlane 远平面距离
        /// @return 生成的透视投影矩阵
        inline Matrix perspective(float fov, float aspectRatio, float nearPlane, float farPlane)
        {
            Matrix Matrix4;
            float f = 1.0f / tanf(fov * 0.5f);

            Matrix4.mat4[0][0] = f / aspectRatio;
            Matrix4.mat4[0][1] = 0.0f;
            Matrix4.mat4[0][2] = 0.0f;
            Matrix4.mat4[0][3] = 0.0f;

            Matrix4.mat4[1][0] = 0.0f;
            Matrix4.mat4[1][1] = f;
            Matrix4.mat4[1][2] = 0.0f;
            Matrix4.mat4[1][3] = 0.0f;

            Matrix4.mat4[2][0] = 0.0f;
            Matrix4.mat4[2][1] = 0.0f;
            Matrix4.mat4[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
            Matrix4.mat4[2][3] = -1.0f;

            Matrix4.mat4[3][0] = 0.0f;
            Matrix4.mat4[3][1] = 0.0f;
            Matrix4.mat4[3][2] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
            Matrix4.mat4[3][3] = 0.0f;
            return Matrix4;
        }

        /// @brief 在给定的矩阵上进行平移变换
        /// @param matrix 要进行平移变换的矩阵
        /// @param x X轴方向上的平移量
        /// @param y Y轴方向上的平移量
        /// @param z Z轴方向上的平移量
        inline int translateMatrix(Matrix *matrix, float x, float y, float z)
        {
            if (matrix == nullptr)
            {
                printf("[%s:%i]translateMatrix() input is NULL!\n", __FILE__, __LINE__);
                return GLMCS_false;
            }
            matrix->mat4[3][0] += x;
            matrix->mat4[3][1] += y;
            matrix->mat4[3][2] += z;
            return GLMCS_ok;
        }

        // 4x4矩阵乘法
        static int matrixMultiply(Matrix *result, Matrix *matrix, float b[4][4])
        {
            if (result == nullptr || matrix == nullptr)
            {
                printf("[%s:%i]matrixMultiply() input is NULL!\n", __FILE__, __LINE__);
                return GLMCS_false;
            }
            float temp[4][4];
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    temp[i][j] = 0.0f;
                    for (int k = 0; k < 4; k++)
                    {
                        temp[i][j] += matrix->mat4[i][k] * b[k][j];
                    }
                }
            }
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    result->mat4[i][j] = temp[i][j];
                }
            }
            return GLMCS_ok;
        }

        /// @brief 绕x/y/z轴旋转,旋转轴x,y,z有且只有一个为1
        /// @param angle 指定旋转角度
        /// @param matrix 输入与输出的矩阵
        /// @param x 旋转轴为x轴
        /// @param y 旋转轴为y轴
        /// @param z 旋转轴为z轴
        /// @return
        inline int rotate(float angle, Matrix *matrix, bool x, bool y, bool z)
        {
            // 不旋转
            if ((int)x + (int)y + (int)z != 1)
            {
                fprintf(stderr, "[%s:%i] [rotate error] Not rotated, make sure that the specified axis of rotation x,y,z has and only one is 1!\n", __FILE__, __LINE__);
                if ((int)x + (int)y + (int)z == 0)
                {
                    return GLMCS_not_rotate;
                }
                return GLMCS_false;
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
            return GLMCS_ok;
        }

        /// @brief 在给定的矩阵上进行缩放变换
        /// @param matrix 要进行缩放变换的矩阵
        /// @param x X轴方向上的缩放因子
        /// @param y Y轴方向上的缩放因子
        /// @param z Z轴方向上的缩放因子
        inline void scaleMatrix(Matrix *matrix, float x, float y, float z)
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
        inline void printMatrix(const Matrix *matrix)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    printf("%.2f\t", matrix->mat4[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }
    } // namespace glmCS

#ifdef __cplusplus
}
#endif

#endif // __CSMATRIX_UTILS_H__
