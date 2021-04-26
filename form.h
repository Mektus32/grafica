#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPicture>
#include <QtCharts>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QChartView>
#include <QGraphicsView>
#include <QMimeData>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QImageReader>

#include <vector>
#include <thread>
#include <vector>
#include <unordered_map>
#include <condition_variable>
#include <memory>
#include <chrono>
#include <iostream>

#include "grafic.h"
#include "threadpool.h"

#define IMAGE_WIDTH 920
#define IMAGE_HEIGHT 800

#define GRAPHIC_SIZE 400

struct Pixel_s
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct ThreadData_s
{
    Pixel_s* m_Data;
    std::size_t m_Len;
};

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();

public slots:
    void UpdatePicture(const std::array<int, 256>& ref_Values);

private slots:
    void dragEnterEvent(QDragEnterEvent *in_Event) override;
    void dropEvent(QDropEvent *in_Event) override;

private:
    bool task(Pixel_s* in_Start, const std::array<int, 256>& in_Values, int in_Len) const;
    void drawHistogram(QtCharts::QChart& in_Histo, const Pixel_s* in_Data, int in_Len);

private:
    QtCharts::QChart m_InitialHistogram;
    QtCharts::QChart m_UpdatedHistogram;
    QLabel m_LabelWithImage;
    QImage m_ResultImage;
    ThreadPool m_Pool;
};
#endif // FORM_H
