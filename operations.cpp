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

QColor Normalize(int r, int g, int b)
{
    if (r < 0)
        r = 0;
    else if (r > 255)
        r = 255;

    if (g < 0)
        g = 0;
    else if (g > 255)
        g = 255;

    if (b < 0)
        b = 0;
    else if (b > 255)
        b = 255;

    return QColor(r, g, b);
}

QColor GetResultPixelColor(QRgb in_Rgb, Channel_e in_Channel, QColor in_Color)
{
    switch (in_Channel)
    {
    case Channel_e::RGB:    return in_Color;
    case Channel_e::R:      return QColor(in_Color.red(), qGreen(in_Rgb), qBlue(in_Rgb));
    case Channel_e::G:      return QColor(qRed(in_Rgb), in_Color.green(), qBlue(in_Rgb));
    case Channel_e::B:      return QColor(qRed(in_Rgb), qGreen(in_Rgb), in_Color.blue());
    case Channel_e::RG:     return QColor(in_Color.red(), in_Color.green(), qBlue(in_Rgb));
    case Channel_e::RB:     return QColor(in_Color.red(), qGreen(in_Rgb), in_Color.blue());
    case Channel_e::GB:     return QColor(qRed(in_Rgb), in_Color.green(), in_Color.blue());
    }

    return QColor(0, 0, 0);
}

void None(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;

    while (true)
    {
        while (x < in_Width)
        {
            color = Normalize(data[i].r * visib,
                              data[i].g * visib,
                              data[i].b * visib);

            currentPixel = GetResultPixelColor(QColor(0, 0, 0).Rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize(data[i].r * visib + (uchar)qRed(rgb),
                              data[i].g * visib + (uchar)qGreen(rgb),
                              data[i].b * visib + (uchar)qBlue(rgb));

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize(data[i].r * visib - (uchar)qRed(rgb) ,
                              data[i].g * visib - (uchar)qGreen(rgb),
                              data[i].b * visib - (uchar)qBlue(rgb));

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize((1 - visib) * (uchar)qRed(rgb)   + ((uchar)qRed(rgb)   * data[i].r * visib / 255),
                              (1 - visib) * (uchar)qGreen(rgb) + ((uchar)qGreen(rgb) * data[i].g * visib / 255),
                              (1 - visib) * (uchar)qBlue(rgb)  + ((uchar)qBlue(rgb)  * data[i].b * visib / 255));

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize(((uchar)qRed(rgb)   + data[i].r * visib) / 2,
                              ((uchar)qGreen(rgb) + data[i].g * visib) / 2,
                              ((uchar)qBlue(rgb)  + data[i].b * visib) / 2);

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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

void Min(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize(std::min((uchar)qRed(rgb), (uchar)(data[i].r * visib)),
                              std::min((uchar)qGreen(rgb), (uchar)(data[i].g * visib)),
                              std::min((uchar)qBlue(rgb), (uchar)(data[i].b * visib)));

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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

void Max(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility)
{
    int y = in_Start / in_Width;
    int x = in_Start % in_Width;
    int i = in_Start;
    double visib = 1. - in_Visibility;
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    QColor color;
    QColor currentPixel;
    QRgb rgb;

    while (true)
    {
        while (x < in_Width)
        {
            rgb = ref_Image.pixelColor(x, y).rgb();

            color = Normalize(std::max((uchar)qRed(rgb), (uchar)(data[i].r * visib)),
                              std::max((uchar)qGreen(rgb), (uchar)(data[i].g * visib)),
                              std::max((uchar)qBlue(rgb), (uchar)(data[i].b * visib)));

            currentPixel = GetResultPixelColor(rgb, in_Picture.GetChannel(), color);

            ref_Image.setPixelColor(x, y, currentPixel);
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
