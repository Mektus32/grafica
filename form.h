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


#include <algorithm>
#include <thread>

#include "picturesettings.h"

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


private:
    int m_MaxWeight;
    int m_MaxHeight;
    std::list<PictureSettings> m_Pictures;
    QVBoxLayout m_Layout;
    QScrollArea m_Area;
    QLabel m_ResultPicture;

};

#endif // FORM_H
