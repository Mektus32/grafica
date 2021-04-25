#include "form.h"

#include <iostream>

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);

    QPixmap background(IMAGE_WIDTH, IMAGE_HEIGHT);
    m_LabelWithImage.setPixmap(background);
    horizontalLayout->addWidget(&m_LabelWithImage);

    QVBoxLayout *verticalLayout = new QVBoxLayout();

    QGraphicsView *view = new QGraphicsView();
    Graphic *grafic = new Graphic(this);
    view->setFixedSize(GRAPHIC_SIZE, GRAPHIC_SIZE);
    view->setScene(grafic);
    view->setSceneRect(0, 0, GRAPHIC_SIZE - 100, GRAPHIC_SIZE - 100);

    verticalLayout->addWidget(view);

    QtCharts::QChartView *newChart = new QtCharts::QChartView();
    newChart->setChart(&m_UpdatedHistogram);
    verticalLayout->addWidget(newChart);

    QtCharts::QChartView *initialChart = new QtCharts::QChartView();
    initialChart->setChart(&m_InitialHistogram);
    verticalLayout->addWidget(initialChart);

    horizontalLayout->addLayout(verticalLayout);
}

Form::~Form()
{

}

void Form::dragEnterEvent(QDragEnterEvent *in_Event)
{
    in_Event->accept();
}

void Form::dropEvent(QDropEvent *in_Event)
{
    QString fileName = in_Event->mimeData()->urls()[0].toLocalFile();

    QByteArray imageFormat = QImageReader::imageFormat(fileName);
    if (!imageFormat.isEmpty())
    {
        QPixmap image(fileName);
        image = image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
        m_ResultImage = image.toImage().convertToFormat(QImage::Format_RGB888);
        m_LabelWithImage.setPixmap(image);
    }
}

#include <chrono>
#include <iostream>

void Form::UpdatePicture(const std::array<int, 256>& ref_Values)
{
    QImage tmp = m_ResultImage.copy();
    Pixel_s* data = (Pixel_s*)tmp.bits();
    std::size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    int len = m_ResultImage.sizeInBytes() / 3;
    Pixel_s* start = data;
    int blockSize = len / threadCount;

    auto first = std::chrono::system_clock::now();

    for (std::size_t i = 0; i < threadCount && len; ++i)
    {
        if (i + 1 != threadCount)
            threads.emplace_back(task, start, std::ref(ref_Values), len / threadCount);
        else
        {
            int last = len - i * blockSize;
            threads.emplace_back(task, start, std::ref(ref_Values), last);
        }
        start += blockSize;
    }

    for (auto& thread : threads)
        thread.join();

    auto end = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - first).count() << std::endl;
    QPixmap pixmap;
    drawHistogram(m_UpdatedHistogram, data, len);
    drawHistogram(m_InitialHistogram, (Pixel_s*)m_ResultImage.bits(), len);
    pixmap.convertFromImage(tmp);
    m_LabelWithImage.setPixmap(std::move(pixmap));
}

void Form::drawHistogram(QtCharts::QChart& in_Histo, const Pixel_s* in_Data, int in_Len)
{
    std::array<int, 256> values{0};

    auto average = [](Pixel_s in_Pixel) -> int {
        return (in_Pixel.r + in_Pixel.g + in_Pixel.b) / 3;
    };

    for (int i = 0; i < in_Len; ++i, ++in_Data)
    {
        values[average(*in_Data)]++;
    }

    QtCharts::QLineSeries *line = new QtCharts::QLineSeries();

    in_Histo.removeAllSeries();

    for (int i = 0; i < 256; ++i)
    {
        line->append(i, values[i]);
    }

    in_Histo.addSeries(line);
}

void task(Pixel_s *in_Start, const std::array<int, 256> &in_Values, int in_Len)
{
    for (int i = 0; i < in_Len; ++i)
    {
        in_Start->r = in_Values[in_Start->r];
        in_Start->g = in_Values[in_Start->g];
        in_Start->b = in_Values[in_Start->b];
        ++in_Start;
    }
}





















