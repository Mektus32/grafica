#include "form.h"

Form::Form(QWidget *parent)
    : QWidget(parent), type(Type_e::LINEAR)
{
    setAcceptDrops(true);

    QHBoxLayout *hLayout = new QHBoxLayout();

    QPixmap pixmap(IMAGE_HEIGHT, IMAGE_WIDTH);
    imageLabel.setPixmap(pixmap);

    hLayout->addWidget(&imageLabel);

    QVBoxLayout *vLayout = new QVBoxLayout();

    QGroupBox *groupBox = new QGroupBox();

    QRadioButton *linearRadioButton = new QRadioButton("Линейная");
    connect(linearRadioButton, &QRadioButton::clicked, [this]()
    {
        type = Type_e::LINEAR;
    });

    QRadioButton *medianRadioButton = new QRadioButton("Медианная");
    connect(medianRadioButton, &QRadioButton::clicked, [this]()
    {
        type = Type_e::MEDIAN;
    });

    QRadioButton *gaussRadioButton = new QRadioButton("Гаусс");
    connect(gaussRadioButton, &QRadioButton::clicked, [this]()
    {
        type = Type_e::GAUSS;
    });

    linearRadioButton->setChecked(true);
    medianRadioButton->setChecked(false);
    gaussRadioButton->setChecked(false);

    QVBoxLayout *radioLayout = new QVBoxLayout();
    radioLayout->addWidget(linearRadioButton);
    radioLayout->addWidget(medianRadioButton);
    radioLayout->addWidget(gaussRadioButton);

    groupBox->setLayout(radioLayout);

    vLayout->addWidget(groupBox);

    QLabel *label = new QLabel("Матрица");
    vLayout->addWidget(label);
    vLayout->addWidget(&text);

    label = new QLabel("Радиус");
    vLayout->addWidget(label);
    vLayout->addWidget(&radius);

    label = new QLabel("Сигма");
    vLayout->addWidget(label);
    vLayout->addWidget(&sigma);

    QPushButton *result = new QPushButton("Преобразовать");
    connect(result, SIGNAL(clicked()), this, SLOT(start()));
    vLayout->addWidget(result);

    hLayout->addLayout(vLayout);

    setLayout(hLayout);

    pool.start(std::thread::hardware_concurrency());
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
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
        image = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
        imageLabel.setPixmap(pixmap);
    }
}

void Form::start()
{
    if (image.isNull())
        return;

    std::size_t threadCount = std::thread::hardware_concurrency();
    std::vector<std::future<bool>> futures;
    futures.reserve(threadCount);

    std::vector<std::vector<double>> matrix;

    QImage tmpImage = image.copy();
    Pixel_s* origImage = (Pixel_s*)image.bits();
    Pixel_s* newImage = (Pixel_s*)tmpImage.bits();
    int len = image.sizeInBytes() / 3;
    int blockSize = len / threadCount;

    switch (type)
    {
        case Type_e::LINEAR:
        case Type_e::GAUSS:
        {
            if (type == Type_e::LINEAR)
            {
                if (!GetMatrix(matrix, text.toPlainText()))
                    return;
            }
            else
            {
                bool isOk = false;
                int rad = radius.text().toInt(&isOk);
                if (!isOk)
                    return;
                int sigm = sigma.text().toInt(&isOk);
                if (!isOk)
                    return;
                GetMatrix(matrix, rad, sigm);
            }

            for (std::size_t i = 0, start = 0; i < threadCount; ++i, start += blockSize)
            {
                std::pair<int, int> indexes{start, i + 1 != threadCount ? start + blockSize : len };
                futures.emplace_back(pool.addTask([=, &matrix]()
                {
                    ApplyMatrix(matrix, origImage, newImage, indexes);
                    return true;
                }));
            }
        } break;
        case Type_e::MEDIAN:
        {
            bool isOk = false;
            int rad = radius.text().toInt(&isOk);
            if (!isOk)
                return;
            for (std::size_t i = 0, start = 0; i < threadCount; ++i, start += blockSize)
            {
                std::pair<int, int> indexes{start, i + 1 != threadCount ? start + blockSize : len };
                futures.emplace_back(pool.addTask([=]()
                {
                    ApplyMedianMatrix(origImage, newImage, rad, indexes);
                    return true;
                }));
            }
        } break;
    }

    for (auto& future : futures)
        future.get();

    QPixmap pixmap;
    pixmap.convertFromImage(tmpImage);
    imageLabel.setPixmap(pixmap);
}
