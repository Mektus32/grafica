#include "operations.h"

void PrintMemory(const std::string& in_FileName, void* in_Data, int in_Size)
{
    std::ofstream file(in_FileName);
    uint32_t* data = (uint32_t*)in_Data;

    for (int i = 0; i < in_Size; ++i)
        file << "0x" << std::hex << data[i] << ' ';
}

Pixel_u Normalize(int a, int r, int g, int b)
{
    Pixel_u color;
    if (a < 0)
        color.a = 0;
    else if (a > 255)
        color.a = 255;
    else
        color.a = a;
    if (r < 0)
        color.r = 0;
    else if (r > 255)
        color.r = 255;
    else
        color.r = r;
    if (g < 0)
        color.g = 0;
    else if (g > 255)
        color.g = 255;
    else
        color.g = g;
    if (b < 0)
        color.b = 0;
    else if (b > 255)
        color.b = 255;
    else
        color.b = b;
    return color;
}

void None(QImage& ref_Image, int in_Start, int in_Lenght, const Pixel_u *in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    double visib = 1. - in_Visibility;
    Pixel_u color;

    while (true)
    {
        while (x < in_Width)
        {
            color = Normalize(in_Picture[i].a,
                              in_Picture[i].r,
                              in_Picture[i].g,
                              in_Picture[i].b);
            ref_Image.setPixelColor(x, y, color.m_Color);
            ++x;
            ++i;
            --in_Lenght;
            if (!in_Lenght)
                return;
        }
        x = 0;
        ++y;
    }
}

void Summ(QImage& ref_Image, int in_Start, int in_Lenght, const Pixel_u *in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_u color;
    QRgb argb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            argb = ref_Image.pixelColor(x, y).rgba();
            color = Normalize(in_Picture[i].a + (uchar)qAlpha(argb),
                              in_Picture[i].r + (uchar)qRed(argb),
                              in_Picture[i].g + (uchar)qGreen(argb),
                              in_Picture[i].b + (uchar)qBlue(argb));
            ref_Image.setPixelColor(x, y, color.m_Color);
            ++x;
            ++i;
            --in_Lenght;
            if (!in_Lenght)
                return;
        }
        x = 0;
        ++y;
    }
}

void Sub(QImage& ref_Image, int in_Start, int in_Lenght, const Pixel_u *in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_u color;
    QRgb argb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            argb = ref_Image.pixelColor(x, y).rgba();
            color = Normalize(in_Picture[i].a - qAlpha(argb),
                              in_Picture[i].r - qRed(argb) ,
                              in_Picture[i].g - qGreen(argb),
                              in_Picture[i].b - qBlue(argb));

            ref_Image.setPixelColor(x, y, color.m_Color);
            ++x;
            ++i;
            --in_Lenght;
            if (!in_Lenght)
                return;
        }
        x = 0;
        ++y;
    }
}

void Multi(QImage& ref_Image, int in_Start, int in_Lenght, const Pixel_u *in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_u color;
    QRgb argb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            argb = ref_Image.pixelColor(x, y).rgba();
            color = Normalize(qAlpha(argb) * in_Picture[i].a,
                              qRed(argb)   * in_Picture[i].r,
                              qGreen(argb) * in_Picture[i].g,
                              qBlue(argb)  * in_Picture[i].b);

            ref_Image.setPixelColor(x, y, color.m_Color);
            ++x;
            ++i;
            --in_Lenght;
            if (!in_Lenght)
                return;
        }
        x = 0;
        ++y;
    }
}

void Average(QImage& ref_Image, int in_Start, int in_Lenght, const Pixel_u *in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_u color;
    QRgb argb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            argb = ref_Image.pixelColor(x, y).rgba();
            color = Normalize((qAlpha(argb) + in_Picture[i].a) / 2,
                              (qRed(argb)   + in_Picture[i].r) / 2,
                              (qGreen(argb) + in_Picture[i].g) / 2,
                              (qBlue(argb)  + in_Picture[i].b) / 2);

            ref_Image.setPixelColor(x, y, color.m_Color);
            ++x;
            ++i;
            --in_Lenght;
            if (!in_Lenght)
                return;
        }
        x = 0;
        ++y;
    }
}
