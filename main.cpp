#include <stdio.h>
#include "matrix_utils.h"

int main()
{
    glmCS::Matrix modelMatrix;
    glmCS::initIdentityMatrix4x4(&modelMatrix); // 单位矩阵

    // 检测返回值
    int ret = 0;

    // 执行平移变换
    glmCS::translateMatrix(&modelMatrix, 4.0f, 5.0f, 6.0f);

    // 绕x/y/z轴旋转
    ret = glmCS::rotate(90, &modelMatrix, 0, 0, 1); // 绕z轴旋转

    // 缩放
    glmCS::scaleMatrix(&modelMatrix, 2.0f, 2.0f, 2.0f);

    // 打印生成的模型矩阵
    printf("Model Matrix result:\n");
    glmCS::printMatrix(&modelMatrix);

    // 将二维数组转为一维数组
    float *p = (float *)modelMatrix.mat4;

    return 0;
}
