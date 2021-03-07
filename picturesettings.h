#ifndef PICTURESETTINGS_H
#define PICTURESETTINGS_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QImage>
#include <QPushButton>

enum class Actions_e
{
    NONE,
    SUMM,
    SUB,
    MULTI,
    AVERAGE
};

union Pixel_u
{
    struct
    {
        uchar r;
        uchar g;
        uchar b;
    };
};

class PictureSettings : public QWidget
{
    Q_OBJECT
public:
    explicit PictureSettings(const QString& in_FileName, QWidget *parent = nullptr);

    void UpdateSizePicture(int in_NewWidth, int in_NewHeight);
    int GetHeight() const;
    int GetWidth() const;
    const Pixel_u*  GetPictureData() const;
    Actions_e GetAction() const;
    double GetVisibility() const;
    void ConvertTo(QImage::Format in_Format);

signals:
    void DeletePicture(PictureSettings* in_pDeletingPicture);

private slots:
    void ChangeAction(int in_ActionIndex);
    void ChangeVisibility(int in_VisibilityValue);
    void DeletePicture();

private:
    QImage m_Picture;
    Actions_e m_Action;
    double m_Visibility;
};

#endif // PICTURESETTINGS_H
