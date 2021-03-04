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
#include <QFile>

#include "picturesettings.h"

class form : public QWidget
{
    Q_OBJECT
public:
    explicit form(QWidget *parent = nullptr);

signals:

private slots:
    void CalculateResultPicture(){};

private:
    std::size_t m_MaxWeight;
    std::size_t m_MaxHeight;

    std::list<PictureSettings> pictures;

};

#endif // FORM_H
