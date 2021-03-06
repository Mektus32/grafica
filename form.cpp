#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollArea>

#include "form.h"

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
    m_ResultPicture.setPixmap(resultPicture);
    hbox->addWidget(&m_ResultPicture);
    hbox->addLayout(vbox);
}

void Form::CalculateResultPicture()
{

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
    int maxWidth = it->GetWidth();

    it = std::max_element(m_Pictures.begin(), m_Pictures.end(), [](const auto& in_LhsPic, const auto& in_RhsPic){
        return in_LhsPic.GetHeight() < in_RhsPic.GetHeight();
    });
    int maxHeight = it->GetHeight();

    for (auto& pic : m_Pictures)
        pic.UpdateSizePicture(maxWidth, maxHeight);
}









