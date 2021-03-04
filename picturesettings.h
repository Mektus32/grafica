#ifndef PICTURESETTINGS_H
#define PICTURESETTINGS_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSlider>

enum class Actions_e
{
    NONE,
    SUMM,
    SUB,
    MULTI
};

class PictureSettings : public QWidget
{
    Q_OBJECT
public:
    explicit PictureSettings(const QString& in_FileName, QWidget *parent = nullptr);

    void UpdateSizePicture(std::size_t in_NewWeight, std::size_t in_NewHeight){};
    const QPixmap& GetPicture() const{};
    Actions_e GetAction() const{};
    std::size_t GetVisibility() const{};

signals:

private slots:
    void ChangeAction(int in_ActionIndex){}; // signal activate, use "int" to show selected index
    void ChangeVisibility(int in_VisibilityValue){}; // signal valueChanged, use "int" to sent current value

private:
    QPixmap m_Picture;
    Actions_e m_Action;
    std::size_t m_Visibility;
};

#endif // PICTURESETTINGS_H
