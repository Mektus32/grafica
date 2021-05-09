#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include <QString>
#include <QStringList>

#define IMAGE_WIDTH 920
#define IMAGE_HEIGHT 800

struct Pixel_s
{
    uchar r = 0;
    uchar g = 0;
    uchar b = 0;

    Pixel_s() = default;

    Pixel_s(uchar r, uchar g, uchar b) : r(r), g(g), b(b)
    {
    }

    Pixel_s operator+=(const Pixel_s& rhs)
    {
        this->r += rhs.r;
        this->g += rhs.g;
        this->b += rhs.b;
        return *this;
    }

    Pixel_s operator*(double rhs)
    {
        return Pixel_s(r * rhs, g * rhs, b * rhs);
    }
};

bool GetMatrix(std::vector<std::vector<double>>& matrix, const QString& str);
bool GetMatrix(std::vector<std::vector<double>>& matrix, int radius, int sigma);
void ApplyMatrix(const std::vector<std::vector<double>>& matrix, const Pixel_s* origImage, Pixel_s* newImage, const std::pair<int, int>& indexs);
void ApplyMedianMatrix(const Pixel_s* origImage, Pixel_s* newImage, int radius, const std::pair<int, int>& indexs);
Pixel_s GetPixelValue(const Pixel_s* image, int x, int y, int dx, int dy);
int GetIndexFromXY(int x, int y);
int GetXFromIndex(int index);
int GetYFromIndex(int index);

#endif // FUNCTIONS_H
