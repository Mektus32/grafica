#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <fstream>
#include <iomanip>

#include "picturesettings.h"

void PrintMemory(const std::string& in_FileName, void* in_Data, int in_Size);
uchar ConverAlphaInUChar(double in_Visibility);
QColor Normalize(int r, int g, int b);
QColor GetResultPixelColor(QRgb in_Rgb, Channel_e in_Channel, QColor in_Color);
void None(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Summ(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Sub(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Multi(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Average(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Min(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);
void Max(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture, int in_Width, double in_Visibility);

#endif // OPERATIONS_H
