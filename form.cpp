#include "form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    QHBoxLayout *h_layout = new QHBoxLayout();

    QPixmap pixmap(window_width, window_height);
    imageLabel.setPixmap(pixmap);

    h_layout->addWidget(&imageLabel);

    fourier.setPixmap(pixmap);
    h_layout->addWidget(&fourier);

    QVBoxLayout *v_layout = new QVBoxLayout();

    v_layout->addWidget(&factors);

    v_layout->addWidget(&circle);

    QPushButton *button = new QPushButton("Calculate");

    connect(button, SIGNAL(clicked()), this, SLOT(calculate()));

    v_layout->addWidget(button);

    h_layout->addLayout(v_layout);

    setLayout(h_layout);

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
        pixmap = pixmap.scaled(window_width, window_height);
        image = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
        imageLabel.setPixmap(pixmap);
    }
}

void Form::calculate() {
    QStringList coeff;
    QStringList circle_data;
    if (!getData(coeff, circle_data))
        return;

    QImage first_step(window_width, window_height, QImage::Format::Format_RGB888);
    QImage second_step(window_width, window_height, QImage::Format::Format_RGB888);
    QImage result(window_width, window_height, QImage::Format::Format_RGB888);

    startThreads(image, first_step, QStringList(), QStringList(), pool);
    startThreads(first_step, second_step, QStringList(), QStringList(), pool, false);

    startThreads(second_step, first_step, coeff, circle_data, pool, false);

    showImage(second_step, fourier, circle_data, true);

    startThreads(first_step, result, QStringList(), QStringList(), pool);

    showImage(result, imageLabel, QStringList());
}

bool Form::getData(QStringList& factors_data, QStringList& circle_data) const noexcept {
    factors_data = factors.text().split(';');

    circle_data = circle.toPlainText().split(';');

    return factors_data.size() == 3 && circle_data.size() == 4;
}













