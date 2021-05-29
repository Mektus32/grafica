#ifndef ALGOS_H
#define ALGOS_H

#include <QWidget>

#define IMAGE_WIDTH 920
#define IMAGE_HEIGHT 840

#define GET_X_FROM_INDEX(i) (i % IMAGE_WIDTH)
#define GET_Y_FROM_INDEX(i) (i / IMAGE_WIDTH)

#include <cmath>
#include <algorithm>
#include <array>

struct Pixel_s
{
    uchar r;
    uchar g;
    uchar b;

    Pixel_s operator=(int t)
    {
        t = this->r > t ? 255 : 0;
        this->r = t;
        this->g = t;
        this->b = t;
        return *this;
    }
};

enum class Algorithms_e
{
    GAVR,
    OTSU,
    NIBLEK,
    SAUVOL,
    WULF,
    ROTA
};

void GetBinariz(Pixel_s* out_NewImage, int in_Len, int t);
std::array<double, 256> GetGistogram(Pixel_s* out_NewImage, int in_Len);
uint64_t Gavr(Pixel_s* out_NewImage, int in_Len);
uint64_t Otsu(Pixel_s* out_NewImage, int in_Len);
void Monochromyze(Pixel_s* out_Image, int in_Len);
void MatWaiting(const Pixel_s* in_Image, int in_A, std::vector<std::pair<double, double>>& ref_MatWaiting);
double GetMaxQ(const std::vector<std::pair<double, double>>& in_MatWaiting, int in_A);
int GetIndexFromXY(int x, int y);
void Rote(const std::vector<std::pair<double, double>>& in_MatWaiting, int in_X, int in_Y, int in_A, std::pair<int, int>& ref_MatWaiting);

#endif // ALGOS_H
