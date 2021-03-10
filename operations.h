#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <fstream>
#include <iomanip>
#include <algorithm>

#include "picturesettings.h"

void PrintMemory(const std::string& in_FileName, void* in_Data, int in_Size);
uchar ConverAlphaInUChar(double in_Visibility);
void UseOperation(const QRgb& in_CurrentPixel, const Pixel_s& in_NewPixel, Actions_e in_Action, QColor& ref_Color);
void UpdateResultImage(QImage& ref_Image, int in_Start, int in_Lenght, const PictureSettings& in_Picture);

#endif // OPERATIONS_H
