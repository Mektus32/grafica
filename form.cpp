#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollArea>

#include "form.h"

std::list<PictureSettings> Form::m_Pictures;

Form::Form(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    QVBoxLayout *vbox = new QVBoxLayout();

    vbox->addWidget(&m_Area);

    QPushButton *addPicture = new QPushButton("Добавить");
    connect(addPicture, SIGNAL(clicked()), this, SLOT(AddPicture()));
    vbox->addWidget(addPicture);

    QPushButton *getResult = new QPushButton("Рассчитать");
    connect(getResult, SIGNAL(clicked()), this, SLOT(CalculateResultPicture()));
    vbox->addWidget(getResult);

    QPixmap resultPicture(1024, 768);
    resultPicture.fill(0xffffff);
    m_ResultPicture.setPixmap(resultPicture);
    hbox->addWidget(&m_ResultPicture);
    hbox->addLayout(vbox);
}

void Form::CalculateThread(QImage& out_Image, int in_Start, int in_Lenght)
{
    for (const auto& pic : m_Pictures)
    {
        switch (pic.GetAction())
        {
            case Actions_e::NONE: None(out_Image, in_Start, in_Lenght, pic, pic.GetWidth(), pic.GetVisibility()); break;
            case Actions_e::SUMM: Summ(out_Image, in_Start, in_Lenght, pic, pic.GetWidth(), pic.GetVisibility()); break;
            case Actions_e::SUB: Sub(out_Image, in_Start, in_Lenght, pic, pic.GetWidth(), pic.GetVisibility()); break;
            case Actions_e::MULTI: Multi(out_Image, in_Start, in_Lenght, pic, pic.GetWidth(), pic.GetVisibility()); break;
            case Actions_e::AVERAGE: Average(out_Image, in_Start, in_Lenght, pic, pic.GetWidth(), pic.GetVisibility()); break;
        }
    }
}

void Form::ShowAndSaveResultImage(const QImage& in_Image)
{
    in_Image.save("Images/" + QDate::currentDate().toString() + ".png");

    QPixmap pixmap;
    pixmap.convertFromImage(in_Image);
    m_ResultPicture.setPixmap(pixmap.scaled(1024, 768));
}

void Form::CalculateResultPicture()
{
    if (m_Pictures.empty())
        return;

    int size = m_MaxHeight * m_MaxWidth;
    int maxThreadCount = std::thread::hardware_concurrency();
    int length = size / maxThreadCount;

    QImage image(m_MaxWidth, m_MaxHeight, QImage::Format_RGB888);
    image.fill(0x000000);
    std::vector<std::thread> threads;
    m_Pictures.reverse();
    for (int i = 0; i < maxThreadCount; ++i)
    {
        if (i + 1 != maxThreadCount)
        {
            threads.emplace_back(Form::CalculateThread, std::ref(image), length * i, length);
        }
        else
        {
            threads.emplace_back(Form::CalculateThread, std::ref(image), length * i, size - length * i);
        }
    }

    for (auto& thread : threads)
        thread.join();
    m_Pictures.reverse();
    ShowAndSaveResultImage(image);
}

void Form::DeletePicture(PictureSettings* in_pPictureSettings)
{
    for (auto it = m_Pictures.begin(); it != m_Pictures.end(); ++it)
    {
        if (&(*it) == in_pPictureSettings)
        {
            m_Pictures.erase(it);
            break;
        }
    }
    m_Area.update();
}

void Form::AddPicture()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg *.bmp)"));
    if (fileName == nullptr)
        return;

    m_Pictures.emplace_back(fileName);
    ResizeAllToMaxPicture();
    auto& pic = m_Pictures.back();
    connect(&pic, SIGNAL(DeletePicture(PictureSettings*)), this, SLOT(DeletePicture(PictureSettings*)));
    m_Layout.addWidget(&pic);

    QWidget *newAreaData = new QWidget();
    newAreaData->setLayout(&m_Layout);
    m_Area.setWidget(newAreaData);
    m_Area.setWidgetResizable(true);
}

void Form::ResizeAllToMaxPicture()
{
    auto it = std::max_element(m_Pictures.begin(), m_Pictures.end(), [](const auto& in_LhsPic, const auto& in_RhsPic){
        return in_LhsPic.GetWidth() < in_RhsPic.GetWidth();
    });
    m_MaxWidth = it->GetWidth();

    it = std::max_element(m_Pictures.begin(), m_Pictures.end(), [](const auto& in_LhsPic, const auto& in_RhsPic){
        return in_LhsPic.GetHeight() < in_RhsPic.GetHeight();
    });
    m_MaxHeight = it->GetHeight();

    for (auto& pic : m_Pictures)
    {
        pic.ConvertTo(QImage::Format_RGB888);
        pic.UpdateSizePicture(m_MaxWidth, m_MaxHeight);
    }
}








