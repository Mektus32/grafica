#include "picturesettings.h"

PictureSettings::PictureSettings(const QString& in_FileName, QWidget *parent /* = nullptr */)
    : QWidget(parent), m_Action(Actions_e::NONE), m_Visibility(0)
{
    m_Picture.load(in_FileName);

    QVBoxLayout *panel = new QVBoxLayout(this);
    QLabel *label = new QLabel();
    QComboBox *comboBox = new QComboBox();
    QSlider *slider = new QSlider(Qt::Horizontal);

    label->setPixmap(m_Picture.scaled(200, 200));
    panel->addWidget(label);
    panel->addWidget(comboBox);
    panel->addWidget(slider);
}
