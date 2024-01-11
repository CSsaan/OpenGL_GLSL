/// @ref core
/// @file truetype.hpp
///
/// @defgroup CS https://github.com/CSsaan/xxx
///
/// @brief The truetype, This file is an encapsulation of the call implementation of [stb_truetype.h],
/// which is used to convert text into a single-channel image array, which is convenient for the display and processing of text in OpenGL and other graphics APIs.
/// USAGE:
///    Include this file in whatever places need to refer to it.
///    [1].Instantiate the object, with [weight & height & font_file] of bitmap:
///        TrueType truetype(500, 100, "/system/bin/fonts/arial.ttf");
///    [2].process input, with [input_characters & font_size]:
///        std::string input = std::to_string(FPS) + " fps";
///        processInput(input, 64.0f);
///    [3].get bitmap weight & height:
///        truetype.getBitmapWH(&w, &h);
///    [4].get font file dir:
///        std::string name = truetype.getTTFdir();
///    [5].get bitmap result:
///        truetype.bitmap;
//////////////////////////////////////////////////////////////////////////////

#ifndef __TRUETYPE_H__
#define __TRUETYPE_H__

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

class TrueType
{
    /* data */
private:
    std::string ttf_dir = "/system/bin/fonts/arial.ttf"; // 默认字体
    stbtt_fontinfo info;
    int bitmap_w = 512; // 位图的宽
    int bitmap_h = 128; // 位图的高
public:
    unsigned char *bitmap = NULL; // 位图

    /* func */
private:
    int init_truetype();
    int ttf2picture(const std::vector<char> &word, float pixels);
    void stringToHex(const std::string &input, std::vector<char> &word);
    void resetBitmap();
    bool isFileExists(const char *tickImagePath);

public:
    TrueType(const std::string &ttf_dir);                             // 默认位图的宽、高
    TrueType(int bitmap_w, int bitmap_h, const std::string &ttf_dir); // 位图的宽、高
    ~TrueType();
    void processInput(const std::string &input, float pixels);
    // TODO: 实现拿参数的函数
    void getBitmapWH(int *w, int *h);
    std::string getTTFdir();
};

//////////////////////////////////////////////////////////////////////////////
TrueType::TrueType(const std::string &ttf_dir)
{
    this->ttf_dir = ttf_dir;
    init_truetype();
}
TrueType::TrueType(int bitmap_w, int bitmap_h, const std::string &ttf_dir)
{
    this->bitmap_w = bitmap_w;
    this->bitmap_h = bitmap_h;
    this->ttf_dir = ttf_dir;
    init_truetype();
}

TrueType::~TrueType()
{
    if (bitmap != nullptr)
    {
        free(bitmap);
    }
}

int TrueType::init_truetype()
{
    /* 加载字体（.ttf）文件 */
    long int size = 0;
    FILE *fontFile = fopen(ttf_dir.c_str(), "rb");
    if (fontFile == NULL)
    {
        printf("Can not open font file:%s\n", ttf_dir.c_str());
        return 0;
    }
    fseek(fontFile, 0, SEEK_END); /* 设置文件指针到文件尾，基于文件尾偏移0字节 */
    size = ftell(fontFile);       /* 获取文件大小（文件尾 - 文件头  单位：字节） */
    fseek(fontFile, 0, SEEK_SET); /* 重新设置文件指针到文件头 */
    unsigned char *fontBuffer = (unsigned char *)calloc(size, sizeof(unsigned char));
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);
    /* 初始化字体 */
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("[%s:%i]stb init font failed\n", __FILE__, __LINE__);
    }
    bitmap = (unsigned char *)calloc(bitmap_w * bitmap_h, sizeof(unsigned char));
    return 1;
}

int TrueType::ttf2picture(const std::vector<char> &word, float pixels)
{
    resetBitmap();
    /* 计算字体缩放 */
    float scale = stbtt_ScaleForPixelHeight(&info, pixels); /* scale = pixels / (ascent - descent) */
    /**
     * 获取垂直方向上的度量
     * ascent：字体从基线到顶部的高度；
     * descent：基线到底部的高度，通常为负值；
     * lineGap：两个字体之间的间距；
     * 行间距为：ascent - descent + lineGap。
     */
    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    /* 根据缩放调整字高 */
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    int x = 0; /*位图的x*/
    /* 循环加载word中每个字符 */
    for (int i = 0; i < (int)word.size(); ++i)
    {
        /**
         * 获取水平方向上的度量
         * advanceWidth：字宽；
         * leftSideBearing：左侧位置；
         */
        int advanceWidth = 0;
        int leftSideBearing = 0;
        stbtt_GetCodepointHMetrics(&info, word[i], &advanceWidth, &leftSideBearing);
        /* 获取字符的边框（边界） */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        /* 计算位图的y (不同字符的高度不同） */
        int y = ascent + c_y1;
        /* 渲染字符 */
        int byteOffset = x + roundf(leftSideBearing * scale) + (y * bitmap_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, word[i]);
        /* 调整x */
        x += roundf(advanceWidth * scale);
        /* 调整字距 */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    if (x == 0)
    {
        printf("[%s:%i]No bitmap write.\n", __FILE__, __LINE__);
        return 0;
    }
    return 1;
}

void TrueType::stringToHex(const std::string &input, std::vector<char> &word)
{
    for (size_t i = 0; i < input.length(); ++i)
    {
        word.push_back(input[i]);
    }
}

void TrueType::resetBitmap()
{
    if (bitmap != nullptr)
    {
        std::fill_n(bitmap, bitmap_w * bitmap_h, 0);
    }
    else
    {
        printf("[%s:%i]resetBitmap() failed, because bitmap = nullptr\n", __FILE__, __LINE__);
    }
}

void TrueType::processInput(const std::string &input, float pixels)
{
    std::vector<char> word;
    stringToHex(input, word);
    ttf2picture(word, pixels);
}

bool TrueType::isFileExists(const char *tickImagePath)
{
    std::ifstream ifile(tickImagePath);
    return bool(ifile.good());
}

void TrueType::getBitmapWH(int *w, int *h)
{
    *w = bitmap_w;
    *h = bitmap_h;
}

std::string TrueType::getTTFdir()
{
    return ttf_dir;
}

#endif // __TRUETYPE_H__
