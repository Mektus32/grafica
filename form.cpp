#include "form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);



    QHBoxLayout *hLayout = new QHBoxLayout();

    m_InputAlgorithm.addItem("1");
    m_InputAlgorithm.addItem("2");
    m_InputAlgorithm.addItem("3");
    m_InputAlgorithm.addItem("4");

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
        m_Image = image.toImage().convertToFormat(QImage::Format_RGB888);
        m_ImageLabel.setPixmap(image);

    }
}

void Form::start()
{
    Algorithms_e algo = static_cast<Algorithms_e>(m_InputAlgorithm.currentIndex());
    int a;
    int b;
    auto data = m_InputData.text().split(' ');
    switch (algo)
    {
        case Algorithms_e::NONE:
        {
            if (checkCountParams(data, 2))
            {
                a = data.at(0).toInt();
                b = data.at(1).toInt();
            }
            else
                return;
        } break;
    }
}

void Form::calculate(Pixel_s* out_NewImage)
{

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




















