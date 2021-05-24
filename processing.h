#include <QImage>
#include <QLabel>

#include <threadpool.h>


enum class Channel {
    R,
    G,
    B
};

struct Pixel {
    uchar r;
    uchar g;
    uchar b;
};

const int window_width = 420;
const int window_height = 920;

int getIndexFromXY(int x, int y);
int getXFormIndex(int i);
int getYFromIndex(int i);
void furie(const Pixel* picture, Pixel* start, int len, int step);
void startThreads(const QImage& image, QImage& tmp,  const QStringList& factors_data,
                  const QStringList& circle_data, ThreadPool& pool, bool is_usual_array = true);
void startThreads(const QImage& image, QImage& tmp, std::pair<int, int> start, std::pair<int, int> finish, const QStringList& circle_data, ThreadPool& pool);
void showImage(const QImage& image, QLabel& label, const QStringList& circle_data, bool ned_circle = false);

