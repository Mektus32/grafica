#include "picturesettings.h"

PictureSettings::PictureSettings(const QString& in_FileName, QWidget *parent /* = nullptr */)
    : QWidget(parent), m_Action(Actions_e::NONE), m_Visibility(0)
{
    m_Picture.load(in_FileName);

    QVBoxLayout *panel = new QVBoxLayout(this);    

    /* Label for picture */
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(m_Picture).scaled(300, 300));
    panel->addWidget(label);

    /* ComboBox for selecting operation */
    QComboBox *comboBoxAction = new QComboBox();
    comboBoxAction->addItem("Нет");
    comboBoxAction->addItem("Сумма");
    comboBoxAction->addItem("Разность");
    comboBoxAction->addItem("Умножение");
    comboBoxAction->addItem("Среднее");
    comboBoxAction->addItem("Минимум");
    comboBoxAction->addItem("Максимум");
    /* connect comboBox and pictureSettings to change action value */
    connect(comboBoxAction, SIGNAL(activated(int)), this, SLOT(ChangeAction(int)));
    panel->addWidget(comboBoxAction);

    /* ComboBox for selecting channel */
    QComboBox *comboBoxChannel = new QComboBox();
    comboBoxChannel->addItem("RGB");
    comboBoxChannel->addItem("R");
    comboBoxChannel->addItem("G");
    comboBoxChannel->addItem("B");
    comboBoxChannel->addItem("RG");
    comboBoxChannel->addItem("RB");
    comboBoxChannel->addItem("GB");
    /* connect comboBox and pictureSettings to change channel value */
    connect(comboBoxChannel, SIGNAL(activated(int)), this, SLOT(ChangeChannel(int)));
    panel->addWidget(comboBoxChannel);

    /* Slider for visibility */
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    QSlider *slider = new QSlider(Qt::Horizontal);
    QLabel *sliderValue = new QLabel("0");
    slider->setTickInterval(1);
    slider->setMaximum(0);
    slider->setMaximum(100);
    /* connect slider and label */
    connect(slider, SIGNAL(valueChanged(int)), sliderValue, SLOT(setNum(int)));
    /* connect slider and pictureSettings to change visibility value */
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(ChangeVisibility(int)));
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(sliderValue);
    panel->addLayout(sliderLayout);

    /* button to delete this picture */
    QPushButton *deleteButton = new QPushButton("Удалить");
    /* connect button to delete picture */
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(DeletePicture()));
    panel->addWidget(deleteButton);

    this->setFixedSize(300, 300);
}

void PictureSettings::UpdateSizePicture(int in_NewWidth, int in_NewHeight)
{
    m_Picture = m_Picture.scaled(in_NewWidth, in_NewHeight);
}

int PictureSettings::GetHeight() const
{
    return m_Picture.height();
}

int PictureSettings::GetWidth() const
{
    return m_Picture.width();
}

const Pixel_s* PictureSettings::GetPictureData() const
{
    return (Pixel_s*)m_Picture.bits();
}

Actions_e PictureSettings::GetAction() const
{
    return m_Action;
}

Channel_e PictureSettings::GetChannel() const
{
    return m_Channel;
}

double PictureSettings::GetVisibility() const
{
    return m_Visibility;
}

void PictureSettings::ConvertTo(QImage::Format in_Format)
{
    m_Picture.convertTo(in_Format);
}

void PictureSettings::ChangeAction(int in_ActionIndex)
{
    m_Action = static_cast<Actions_e>(in_ActionIndex);
}

void PictureSettings::ChangeChannel(int in_ChannelIndex)
{
    m_Channel = static_cast<Channel_e>(in_ChannelIndex);
}

void PictureSettings::ChangeVisibility(int in_VisibilityValue)
{
    m_Visibility = in_VisibilityValue / 100.;
}

void PictureSettings::DeletePicture()
{
    emit DeletePicture(this);
}

QColor Normalize(int r, int g, int b)
{

    if (r < 0)
        r = 0;
    else if (r > 255)
        r = 255;
    if (g < 0)
        g = 0;
    else if (g > 255)
        g = 255;
    if (b < 0)
       b = 0;
    else if (b > 255)
        b = 255;
    return QColor(r, g, b);
}

QColor operator+(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return Normalize(qRed(in_Lhs)   + in_Rhs.r,
                     qGreen(in_Lhs) + in_Rhs.g,
                     qBlue(in_Lhs)  + in_Rhs.b);
}

QColor operator-(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return Normalize(qRed(in_Lhs)   - in_Rhs.r,
                     qGreen(in_Lhs) - in_Rhs.g,
                     qBlue(in_Lhs)  - in_Rhs.b);
}

QColor operator*(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return Normalize(qRed(in_Lhs)   * in_Rhs.r,
                     qGreen(in_Lhs) * in_Rhs.g,
                     qBlue(in_Lhs)  * in_Rhs.b);
}

Pixel_s operator*(const Pixel_s& in_Lhs, const Visib_s& in_Rhs)
{
    return { static_cast<uchar>(in_Lhs.r * in_Rhs.r),
             static_cast<uchar>(in_Lhs.r * in_Rhs.r),
             static_cast<uchar>(in_Lhs.r * in_Rhs.r) };
}

QColor operator/(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return Normalize((qRed(in_Lhs)   + in_Rhs.r) / 2,
                     (qGreen(in_Lhs) + in_Rhs.g) / 2,
                     (qBlue(in_Lhs)  + in_Rhs.b) / 2);
}

QColor min(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return { std::min(static_cast<uchar>(qRed(in_Lhs)),   in_Rhs.r),
             std::min(static_cast<uchar>(qGreen(in_Lhs)), in_Rhs.g),
             std::min(static_cast<uchar>(qBlue(in_Lhs)),  in_Rhs.b) };
}

QColor max(const QRgb& in_Lhs, const Pixel_s& in_Rhs)
{
    return { std::max(static_cast<uchar>(qRed(in_Lhs)),   in_Rhs.r),
             std::max(static_cast<uchar>(qGreen(in_Lhs)), in_Rhs.g),
             std::max(static_cast<uchar>(qBlue(in_Lhs)),  in_Rhs.b) };
}

























