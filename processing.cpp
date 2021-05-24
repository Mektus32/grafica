#include "processing.h"

int getIndexFromXY(int x, int y) {
    return y * window_width + x;
}

int getXFromIndex(int i) {
    return i % window_width;
}

int getYFromIndex(int i) {
    return i / window_width;
}

void furie(const Pixel* picture, Pixel* start, int len, int step) {
//    for (int i = 0, j = static_cast<int>(channel); i < len; ++i) {

//    }
}

void startThreads(const QImage& image, QImage& tmp,  const QStringList& factors_data,
                  const QStringList& circle_data, ThreadPool& pool, bool is_usual_array) {
    const Pixel *data = reinterpret_cast<Pixel*>(const_cast<QImage&>(image).bits());
    int len = image.sizeInBytes() / 3;

    auto start = reinterpret_cast<Pixel*>(tmp.bits());
    std::vector<std::future<bool>> futures;
    auto threads = std::thread::hardware_concurrency();
    auto count = 0;

    if (is_usual_array) {
        futures.reserve(window_height);
        for (std::size_t i = 0; i < threads; ++i) {
            if (i + 1 != threads) {
                futures.emplace_back(pool.addTask([=]() -> bool {
                    furie(data, start, count, 1);
                    return true;
                }));
            } else {
                futures.emplace_back(pool.addTask([=]() -> bool {
                    furie(data, start, len - count, 1);
                    return true;
                }));
            }

            count += window_width;
        }
    } else {
        futures.reserve(window_width);
        for (std::size_t i = 0; i < threads; ++i) {
            if (i + 1 != threads) {
                futures.emplace_back(pool.addTask([=]() -> bool {
                    furie(data, start, count, window_width);
                    return true;
                }));
            } else {
                futures.emplace_back(pool.addTask([=]() -> bool {
                    furie(data, start, len - count, window_width);
                    return true;
                }));
            }

            count += window_height;
        }
    }

    for (auto& future : futures)
        future.get();
}

void startThreads(const QImage& image, QImage& tmp, std::pair<int, int> start, std::pair<int, int> finish, const QStringList& circle_data, ThreadPool& pool) {
    const Pixel *data = reinterpret_cast<Pixel*>(const_cast<QImage&>(image).bits()) + getIndexFromXY(start.first, start.second);
    int len = getIndexFromXY(finish.first, finish.second) - getIndexFromXY(start.first, start.second);

    auto begin = reinterpret_cast<Pixel*>(tmp.bits()) + getIndexFromXY(start.first, start.second);
    std::vector<std::future<bool>> futures;
    auto threads = std::thread::hardware_concurrency();
    futures.reserve(len / threads);
    auto block_size = len / threads;
    auto count = 0;

    for (std::size_t i = 0; i < threads; ++i) {
        if (i + 1 != threads) {
            futures.emplace_back(pool.addTask([=]() -> bool {
                //furie(data, begin, count, 1);
                return true;
            }));
        } else {
            futures.emplace_back(pool.addTask([=]() -> bool {
                //furie(data, begin, len - count, 1);
                return true;
            }));
        }

        count += block_size;
    }
}

void showImage(const QImage& image, QLabel& label, const QStringList& circle_data, bool need_circle) {
    if (need_circle) {

    } else {
        QPixmap pixmap;
        pixmap.convertFromImage(image);
        label.setPixmap(pixmap);
    }
}
