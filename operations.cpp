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

void UseOperation(const QRgb& in_CurrentPixel, const Pixel_s& in_NewPixel, Actions_e in_Action, QColor& ref_Color)
{
    switch (in_Action)
    {
        case Actions_e::NONE:       ref_Color = QColor(in_NewPixel.r, in_NewPixel.g, in_NewPixel.b); break;
        case Actions_e::SUMM:       ref_Color = in_CurrentPixel + in_NewPixel;      break;
        case Actions_e::SUB:        ref_Color = in_CurrentPixel - in_NewPixel;      break;
        case Actions_e::MULTI:      ref_Color = in_CurrentPixel * in_NewPixel;      break;
        case Actions_e::AVERAGE:    ref_Color = in_CurrentPixel / in_NewPixel;      break;
        case Actions_e::MIN:        ref_Color = min(in_CurrentPixel, in_NewPixel);  break;
        case Actions_e::MAX:        ref_Color = max(in_CurrentPixel, in_NewPixel);  break;
    }
}

void UpdateResultImage(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture)
{
    int     y = in_Start / in_Picture.GetWidth();
    int     x = in_Start % in_Picture.GetWidth();
    int     i = in_Start;
    //double  visib = 1 - in_Picture.GetVisibility();
    Pixel_s *data = (Pixel_s*)in_Picture.GetPictureData();
    Pixel_s newPixel;
    QColor  resultPixel;
    QRgb    currentPixel;

    while (true)
    {
        while (x < in_Picture.GetWidth())
        {
            newPixel = data[i];// * Visib_s(visib);
            UseOperation(ref_Image.pixelColor(x, y).Rgb, newPixel, in_Picture.GetAction(), resultPixel);
            switch (in_Picture.GetChannel())
            {
                case Channel_e::RGB: ref_Image.setPixelColor(x, y, resultPixel); break;
                case Channel_e::R:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(resultPixel.red(), qGreen(currentPixel), qBlue(currentPixel)));
                } break;
                case Channel_e::G:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(qRed(currentPixel), resultPixel.green(), qBlue(currentPixel)));
                } break;
                case Channel_e::B:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(qRed(currentPixel), qGreen(currentPixel), resultPixel.blue()));
                } break;
                case Channel_e::RG:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(resultPixel.red(), resultPixel.green(), qBlue(currentPixel)));
                } break;
                case Channel_e::RB:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(resultPixel.red(), qGreen(currentPixel), resultPixel.blue()));
                } break;
                case Channel_e::GB:
                {
                    currentPixel = ref_Image.pixelColor(x, y).Rgb;
                    ref_Image.setPixelColor(x, y, QColor(qRed(currentPixel), resultPixel.green(), resultPixel.blue()));
                } break;
            }
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


