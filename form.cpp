#include "form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    QHBoxLayout *hLayout = new QHBoxLayout();

    m_InputAlgorithm.addItem("Гаврилова");
    m_InputAlgorithm.addItem("Отсу");
    m_InputAlgorithm.addItem("Ниблека");
    m_InputAlgorithm.addItem("Сауволы");
    m_InputAlgorithm.addItem("Кристиана Вульфа");
    m_InputAlgorithm.addItem("Брэдли-Рота");

    hLayout->addWidget(&m_InputAlgorithm);

    hLayout->addWidget(&m_InputData);

    QPushButton *button = new QPushButton("Посчитать");

    connect(button, SIGNAL(clicked()), this, SLOT(start()));

    hLayout->addWidget(button);

    QVBoxLayout *vLayout = new QVBoxLayout();

    vLayout->addLayout(hLayout);

    QPixmap pixmap(IMAGE_WIDTH, IMAGE_HEIGHT);
    m_ImageLabel.setPixmap(pixmap);
    vLayout->addWidget(&m_ImageLabel);

    setLayout(vLayout);

    m_Pool.start(std::thread::hardware_concurrency());
    m_ThreadsData.reserve(std::thread::hardware_concurrency());
}

Form::~Form()
{
}

void Form::dragEnterEvent(QDragEnterEvent *in_Event)
{
    in_Event->accept();
}


#include <iostream>
#include <fstream>

void Form::dropEvent(QDropEvent *in_Event)
{
    QString fileName = in_Event->mimeData()->urls()[0].toLocalFile();

    QByteArray imageFormat = QImageReader::imageFormat(fileName);
    if (!imageFormat.isEmpty())
    {
        QPixmap image(fileName);
        image = image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
        m_Image = image.toImage().convertToFormat(QImage::Format_RGB888);
        m_ImageLabel.setPixmap(image);

        std::vector<std::future<uint64_t>> futures;
        auto data = (Pixel_s*)m_Image.bits();
        std::size_t threadCount = std::thread::hardware_concurrency();
        futures.reserve(threadCount);
        m_ThreadsData.clear();
        std::size_t len = m_Image.sizeInBytes() / 3;
        std::size_t blockSize = len / threadCount;
        std::size_t start = 0;
        for (std::size_t i = 0; i < threadCount; ++i)
        {
            std::size_t size;
            if (i + 1 != threadCount)
            {
                size = blockSize;
            }
            else
            {
                size = len - start;
            }


            futures.emplace_back(m_Pool.addTask([data, start, size]() -> uint64_t
            {
                Monochromyze(data + start, size);
                return 0;
            }));

            m_ThreadsData.emplace_back(start, size);
            start += blockSize;
        }
        for (auto& future : futures)
            future.get();

        QPixmap pixmap;
        pixmap.convertFromImage(m_Image);
        m_ImageLabel.setPixmap(pixmap);
    }
}

void Form::start()
{
    if (m_Image.isNull())
        return;
    Algorithms_e algo = static_cast<Algorithms_e>(m_InputAlgorithm.currentIndex());
    auto str = m_InputData.text().split(' ');
    QImage tmp = m_Image.copy();
    auto data = (Pixel_s*)tmp.bits();
    switch (algo)
    {
        case Algorithms_e::GAVR:
        {
            std::vector<std::future<uint64_t>> futures;
            futures.reserve(std::thread::hardware_concurrency());
            for (std::size_t i = 0; i < m_ThreadsData.size(); ++i)
            {
                auto start = m_ThreadsData[i].m_Start;
                auto size = m_ThreadsData[i].m_Len;
                futures.emplace_back(m_Pool.addTask([data, start, size]() -> uint64_t
                {
                    return Gavr(data + start, size);
                }));
            }

            uint64_t sum = 0;
            for (auto& future : futures)
                sum += future.get();
            auto t = sum / (tmp.sizeInBytes() / 3);

            futures.clear();

            for (std::size_t i = 0; i < m_ThreadsData.size(); ++i)
            {
                auto start = m_ThreadsData[i].m_Start;
                auto size = m_ThreadsData[i].m_Len;
                auto tmpData = data + start;

                futures.emplace_back(m_Pool.addTask([tmpData, size, t]() -> uint64_t
                {
                    GetBinariz(tmpData, size, t);
                    return 0;
                }));
            }

            for (auto& future : futures)
                future.get();

        } break;
        case Algorithms_e::OTSU:
        {
            std::vector<std::future<uint64_t>> futures;
            futures.reserve(std::thread::hardware_concurrency());

            auto t = Otsu(data, tmp.sizeInBytes() / 3);

            futures.clear();

            for (std::size_t i = 0; i < m_ThreadsData.size(); ++i)
            {
                auto start = m_ThreadsData[i].m_Start;
                auto size = m_ThreadsData[i].m_Len;
                auto tmpData = data + start;

                futures.emplace_back(m_Pool.addTask([tmpData, size, t]() -> uint64_t
                {
                    GetBinariz(tmpData, size, t);
                    return 0;
                }));
            }

            for (auto& future : futures)
                future.get();

        } break;
        case Algorithms_e::NIBLEK:
        case Algorithms_e::SAUVOL:
        case Algorithms_e::WULF:
        case Algorithms_e::ROTA:
        {
            if (!checkCountParams(str, 2))
                return;
            calculate(data, tmp.sizeInBytes() / 3, str[0].toInt(), str[1].toDouble(), algo);
        } break;
    }

    QPixmap pixmap;
    pixmap.convertFromImage(tmp);
    m_ImageLabel.setPixmap(pixmap);
}



void Form::calculate(Pixel_s* out_NewImage, int in_Len, int in_A, double in_K, Algorithms_e in_Algo)
{
    std::vector<std::pair<double, double>> matWaiting(in_Len);
    MatWaiting(out_NewImage, in_A, matWaiting);
    double maxQ = GetMaxQ(matWaiting, in_A);

    std::vector<std::future<uint64_t>> futures;
    futures.reserve(std::thread::hardware_concurrency());

    for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
        futures.emplace_back(m_Pool.addTask([=]() -> uint64_t
        {
            auto data = out_NewImage + m_ThreadsData[i].m_Start;
            for (std::size_t index = m_ThreadsData[i].m_Start; index < m_ThreadsData[i].m_Start + m_ThreadsData[i].m_Len; ++index, ++data)
            {
                int t = 0;
                std::pair<int, int> m;
                Rote(matWaiting, GET_X_FROM_INDEX(index), GET_Y_FROM_INDEX(index), in_A, m);
                double d = m.second - std::pow(m.first, 2);
                double q = std::sqrt(d);
                switch (in_Algo) {
                    case Algorithms_e::NIBLEK:
                    {
                        t = std::round(m.first + in_K * q);
                    } break;

                    case Algorithms_e::SAUVOL:
                    {
                        int r = 128;
                        t = m.first * (1 + in_K * ((q / r) - 1));
                    } break;
                    case Algorithms_e::WULF:
                    {
                        t = std::round((1 - in_K) * m.first + in_K * 0 + std::abs(in_K * q * (m.first - 0) / (maxQ * 40)));
                    } break;
                    case Algorithms_e::ROTA:
                    {
                        t = m.first * (1 - in_K);
                    } break;
                }

                *data = t;
            }
            return 0;
        }));
    }

    for (auto& future : futures)
        future.get();
}

bool Form::checkCountParams(const QStringList& in_List, int in_Needed) const
{
    if (in_List.size() < in_Needed)
    {
        QErrorMessage message;
        message.showMessage("Мore parameters were expected\n");
        return false;
    }
    return true;
}


















