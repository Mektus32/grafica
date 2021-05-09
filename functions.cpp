#include "functions.h"

bool GetMatrix(std::vector<std::vector<double>>& matrix, const QString& str)
{
    auto rows = str.split('\n');
    int colCount = 0;

    if (rows.size() % 2 == 0)
        return false;

    matrix.resize(rows.size());

    for (std::size_t i = 0; i < matrix.size(); ++i)
    {
        auto values = rows[i].split(' ');

        if (i != 0 && (colCount != values.size() || colCount % 2 == 0))
            return false;

        colCount = values.size();
        matrix[i].resize(colCount);
        for (std::size_t j = 0; j < matrix[i].size(); ++j)
        {
            bool isOk = false;
            matrix[i][j] = values[j].toDouble(&isOk);
            if (!isOk)
            {
                double num;
                double delim;
                auto fraction = values[j].split('/');
                if (fraction.size() != 2)
                    return false;
                num = fraction[0].toDouble(&isOk);
                if (!isOk)
                    return false;
                delim = fraction[1].toDouble(&isOk);
                if (!isOk || delim == 0)
                    return false;
                matrix[i][j] = num / delim;
            }
        }
    }

    return true;
}

bool GetMatrix(std::vector<std::vector<double>>& matrix, int radius, int sigma)
{
    double s = 0;
    double g;

    auto sig_sqr = 2.0 * std::pow(sigma, 2);
    auto pi_siq_sqr = sig_sqr * M_PI;

    matrix.resize(radius * 2 + 1);
    for (auto& vec : matrix)
        vec.resize(radius * 2 + 1);

    for (int i = -radius, y = 0; i <= radius; ++i, ++y)
    {
        for (int j = -radius, x = 0; j <= radius; ++j, ++x)
        {
            g = 1.0 / pi_siq_sqr * std::exp(-1.0 * (std::pow(i, 2) + std::pow(j, 2)) / (sig_sqr));
            s += g;

            matrix[y][x] = g;
        }
    }

    return true;
}

void ApplyMatrix(const std::vector<std::vector<double>>& matrix, const Pixel_s* origImage, Pixel_s* newImage, const std::pair<int, int>& indexs)
{
    int h = matrix.size() / 2;
    int w = matrix[0].size() / 2;

    for (int index = indexs.first; index < indexs.second; ++index)
    {
        Pixel_s result;

        for (int dy = -h, y = 0; dy <= h; ++dy, ++y)
        {
            for (int dx = -w, x = 0; dx <= w; ++dx, ++x)
            {
                result += GetPixelValue(origImage, GetXFromIndex(index), GetYFromIndex(index), dx, dy) * matrix[y][x];
            }
        }

        newImage[index] = result;
    }
}

void ApplyMedianMatrix(const Pixel_s* origImage, Pixel_s* newImage, int radius, const std::pair<int, int>& indexs)
{
    int elementsCount = std::pow(radius * 2 + 1, 2);
    std::vector<uchar> r(elementsCount);
    std::vector<uchar> g(elementsCount);
    std::vector<uchar> b(elementsCount);

    for (int index = indexs.first; index < indexs.second; ++index)
    {
        for (int dy = -radius, i = 0; dy <= radius; ++dy)
        {
            for (int dx = -radius; dx <= radius; ++dx, ++i)
            {
                auto result = GetPixelValue(origImage, GetXFromIndex(index), GetYFromIndex(index), dx, dy);
                r[i] = result.r;
                g[i] = result.g;
                b[i] = result.b;
            }
        }

        std::sort(r.begin(), r.end());
        std::sort(g.begin(), g.end());
        std::sort(b.begin(), b.end());

        auto center = r.size() / 2;

        newImage[index].r = r[center];
        newImage[index].g = g[center];
        newImage[index].b = b[center];
    }
}

Pixel_s GetPixelValue(const Pixel_s* image, int x, int y, int dx, int dy)
{
    if (x + dx < 0 || x + dx >= IMAGE_WIDTH)
        x -= dx;
    else
        x += dx;
    if (y + dy < 0 || y + dy >= IMAGE_HEIGHT)
        y -= dy;
    else
        y += dy;
    return image[GetIndexFromXY(x, y)];
}

int GetIndexFromXY(int x, int y)
{
    return y * IMAGE_WIDTH + x;
}

int GetXFromIndex(int index)
{
    return index % IMAGE_WIDTH;
}

int GetYFromIndex(int index)
{
    return index / IMAGE_WIDTH;
}
