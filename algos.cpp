#include "algos.h"

void GetBinariz(Pixel_s* out_NewImage, int in_Len, int t)
{
    for(int i = 0; i < in_Len; ++i)
    {
        out_NewImage[i] = t;
    }
}

std::array<double, 256> GetGistogram(Pixel_s* out_NewImage, int in_Len)
{
    std::array<double, 256> gisto{0};
    for(int i = 0; i < in_Len; ++i)
    {
        int pix = (out_NewImage[i].r + out_NewImage[i].g + out_NewImage[i].b) / 3;
        gisto[pix]++;
    }
    return gisto;
}

uint64_t Gavr(Pixel_s* out_NewImage, int in_Len)
{
    uint64_t sum = 0;
    for(int i = 0; i < in_Len; ++i)
    {
        sum += out_NewImage[i].r;
    }
    return sum;
}

#include <fstream>

uint64_t Otsu(Pixel_s* out_NewImage, int in_Len)
{
    std::array<double, 256> gisto = GetGistogram(out_NewImage, in_Len);
    for(int i = 0; i < 256; ++i)
    {
        gisto[i] /= in_Len;
    }

    double maxQB = 0;
    int t = 0;

    for(int i = 1; i < 256; ++i)
    {
        double w1 = 0;
        double w2 = 0;
        double m1 = 0;
        double m2 = 0;
        double mT = 0;
        double qB = 0;

        for(int j = 0; j <= i - 1; ++j)
        {
            w1 += gisto[j];
        }
        w2 = 1 - w1;
        for(int j = 0; j <= i - 1; ++j)
        {
            m1 += gisto[j] * j;
        }
        m1 /= w1;
        for(int j = 0; j <= 255; ++j)
        {
            mT += gisto[j] * j;
        }
        m2 = (mT - m1 * w1) / w2;
        int m = (int)(m1 - m2);
        qB = w1 * w2 * m * m;
        if(qB > maxQB)
        {
            maxQB = qB;
            t = i;
        }
    }
    return t;
}

void Monochromyze(Pixel_s* out_Image, int in_Len)
{
    for (int i = 0; i < in_Len; ++i, ++out_Image)
    {
        auto value = (out_Image->r + out_Image->g + out_Image->b) / 3;
        out_Image->r = (uchar)value;
        out_Image->g = (uchar)value;
        out_Image->b = (uchar)value;
    }
}

void MatWaiting(const Pixel_s* in_Image, int in_A, std::vector<std::pair<double, double>>& ref_MatWaiting)
{
    std::vector<int> square(std::pow(in_A, 2));
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            auto i = in_Image[GetIndexFromXY(x, y)].r;
            auto s1 = x - 1 < 0 || y < 0 || x - 1 >= IMAGE_WIDTH || y >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x - 1, y)].first;
            auto s2 = x < 0 || y - 1 < 0 || x >= IMAGE_WIDTH || y - 1 >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x, y - 1)].first;
            auto s3 = x - 1 < 0 || y - 1 < 0 || x - 1 >= IMAGE_WIDTH || y - 1 >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x - 1, y - 1)].first;
            ref_MatWaiting[GetIndexFromXY(x, y)].first = i + s1 + s2 - s3;
            s1 = x - 1 < 0 || y < 0 || x - 1 >= IMAGE_WIDTH || y >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x - 1, y)].second;
            s2 = x < 0 || y - 1 < 0 || x >= IMAGE_WIDTH || y - 1 >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x, y - 1)].second;
            s3 = x - 1 < 0 || y - 1 < 0 || x - 1 >= IMAGE_WIDTH || y - 1 >= IMAGE_HEIGHT ? 0 : ref_MatWaiting[GetIndexFromXY(x - 1, y - 1)].second;
            ref_MatWaiting[GetIndexFromXY(x, y)].second = std::pow(i, 2) + s1 + s2 - s3;
        }
    }
}

double GetMaxQ(const std::vector<std::pair<double, double>>& in_MatWaiting, int in_A)
{
    double maxQ = -1;
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            std::pair<int, int> m;
            Rote(in_MatWaiting, x, y, in_A, m);
            double d = m.second - std::pow(m.first, 2);
            double q = std::sqrt(d);
            maxQ = std::max(maxQ, q);
        }
    }
    return maxQ;
}

int GetIndexFromXY(int x, int y)
{
    if (x < 0 || y < 0 || x >= IMAGE_WIDTH || y >= IMAGE_HEIGHT)
        return 0;
    return y * IMAGE_WIDTH + x;
}

void Rote(const std::vector<std::pair<double, double>>& in_MatWaiting, int in_X, int in_Y, int in_A, std::pair<int, int>& ref_MatWaiting)
{
    int h = in_A / 2;
    auto y1 = (in_Y - h - 1 < 0) || (in_X - h - 1 < 0) || (in_Y - h - 1 >= IMAGE_HEIGHT) || (in_X - h - 1 >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X - h - 1, in_Y - h - 1)].first; //done
    auto y2 = (in_Y - h - 1 < 0) || (in_X + h < 0) || (in_Y - h - 1 >= IMAGE_HEIGHT) || (in_X + h >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X + h, in_Y - h - 1)].first; //done
    auto x1 = (in_Y + h < 0) || (in_X - h - 1 < 0) || (in_Y + h >= IMAGE_HEIGHT) || (in_X - h - 1 >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X - h - 1, in_Y + h)].first;//done
    auto x2 = (in_Y + h < 0) || (in_X + h < 0) || (in_Y + h >= IMAGE_HEIGHT) || (in_X + h >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X + h, in_Y + h)].first; //done

    ref_MatWaiting.first = (int)(x2 + y1 - x1 - y2) / std::pow(in_A, 2);

    y1 = (in_Y - h - 1 < 0) || (in_X - h - 1 < 0) || (in_Y - h - 1 >= IMAGE_HEIGHT) || (in_X - h - 1 >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X - h - 1, in_Y - h - 1)].second; //done
    y2 = (in_Y - h - 1 < 0) || (in_X + h < 0) || (in_Y - h - 1 >= IMAGE_HEIGHT) || (in_X + h >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X + h, in_Y - h - 1)].second; //done
    x1 = (in_Y + h < 0) || (in_X - h - 1 < 0) || (in_Y + h >= IMAGE_HEIGHT) || (in_X - h - 1 >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X - h - 1, in_Y + h)].second;//done
    x2 = (in_Y + h < 0) || (in_X + h < 0) || (in_Y + h >= IMAGE_HEIGHT) || (in_X + h >= IMAGE_WIDTH) ? 0 : in_MatWaiting[GetIndexFromXY(in_X + h, in_Y + h)].second; //done

    ref_MatWaiting.second = (int)(x2 + y1 - x1 - y2) / std::pow(in_A, 2);
}
