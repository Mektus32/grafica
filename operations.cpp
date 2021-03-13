#include "operations.h"

void PrintMemory(const std::string& in_FileName, void* in_Data, int in_Size)
{
    std::ofstream file(in_FileName);
    uint32_t* data = (uint32_t*)in_Data;

    for (int i = 0; i < in_Size; ++i)
        file << "0x" << std::hex << data[i] << ' ';
}

uchar ConverAlphaInUChar(double in_Visibility)
{
    uchar tmp = 255 * in_Visibility;
    return tmp;
}

Pixel_s Normalize(int r, int g, int b)
{
    Pixel_s color;
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

void None(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    double visib = 1. - in_Visibility;
    Pixel_s color;
    QColor currentPixel;

    while (true)
    {
        while (x < in_Width)
        {
            color = Normalize(data[i].r * visib,
                              data[i].g * visib,
                              data[i].b * visib);
            //QColor tmpColor(color.r, color.g, color.b);
            ref_Image.setPixelColor(x, y, tmpColor);
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

void Summ(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    Pixel_s color;
    QRgb rgb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();
            color = Normalize(data[i].r * visib + (uchar)qRed(rgb),
                              data[i].g * visib + (uchar)qGreen(rgb),
                              data[i].b * visib + (uchar)qBlue(rgb));
            QColor tmpColor(color.r, color.g, color.b);
            ref_Image.setPixelColor(x, y, tmpColor);
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

void Sub(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    Pixel_s color;
    QRgb rgb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();
            color = Normalize(data[i].r * visib - (uchar)qRed(rgb) ,
                              data[i].g * visib - (uchar)qGreen(rgb),
                              data[i].b * visib - (uchar)qBlue(rgb));

            QColor tmpColor(color.r, color.g, color.b);
            ref_Image.setPixelColor(x, y, tmpColor);
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

void Multi(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_s color;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QRgb rgb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();
            color = Normalize((uchar)qRed(rgb)   * data[i].r * visib,
                              (uchar)qGreen(rgb) * data[i].g * visib,
                              (uchar)qBlue(rgb)  * data[i].b * visib);

            QColor tmpColor(color.r, color.g, color.b);
            ref_Image.setPixelColor(x, y, tmpColor);
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

void Average(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    Pixel_s color;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QRgb rgb;
    double visib = 1. - in_Visibility;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();
            color = Normalize(((uchar)qRed(rgb)   + data[i].r * visib) / 2,
                              ((uchar)qGreen(rgb) + data[i].g * visib) / 2,
                              ((uchar)qBlue(rgb)  + data[i].b * visib) / 2);

            QColor tmpColor(color.r, color.g, color.b);
            ref_Image.setPixelColor(x, y, tmpColor);
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
