#include "form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);

    QPixmap background(1000, 800);
    m_LabelWithImage.setPixmap(background);
    horizontalLayout->addWidget(&m_LabelWithImage);

    QVBoxLayout *verticalLayout = new QVBoxLayout();

    Grafic *grafic = new Grafic(this);
    verticalLayout->addWidget(grafic);

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

    QPixmap image(fileName);
    m_ResultImage = image.toImage();
    m_LabelWithImage.setPixmap(image);
}


//void Form::mouseReleaseEvent(QMouseEvent *in_Event)
//{

//}

void Form::UpdatePicture(QPoint in_Point)
{

}
