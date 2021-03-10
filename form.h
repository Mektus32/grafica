#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QColor>
#include <QDate>

#include <algorithm>
#include <thread>
#include <vector>

#include "picturesettings.h"
#include "operations.h"

#define BACKGROUND 0x000000

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = nullptr);

signals:

private slots:
    void CalculateResultPicture();
    void DeletePicture(PictureSettings* in_pDeletingPicture);
    void AddPicture();

private:
    void ResizeAllToMaxPicture();
    static void CalculateThread(QImage& out_Image, int in_Start, int in_Lenght);
    void ShowAndSaveResultImage(const QImage& in_Image);

private:
    int m_MaxWidth;
    int m_MaxHeight;
    static std::list<PictureSettings> m_Pictures;
    QVBoxLayout m_Layout;
    QScrollArea m_Area;
    QLabel m_ResultPicture;
};

#endif // FORM_H
