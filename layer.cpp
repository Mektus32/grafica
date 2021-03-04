#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "layer.h"

layer::layer(QWidget *parent) : QWidget(parent)
{
    //QLabel *befor = new QLabel("here", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *after = new QLabel(this);
    QLabel *after2 = new QLabel(this);
    QLabel *after3 = new QLabel(this);
    QLabel *after4 = new QLabel(this);
    QLabel *after5 = new QLabel(this);
    QPixmap pixmap("1.jpg");
    QSize size(100, 100);
    //befor->setPixmap(pixmap);
    after->setPixmap(pixmap.scaled(size));
    after2->setPixmap(pixmap.scaled(size));
    after3->setPixmap(pixmap.scaled(size));
    after4->setPixmap(pixmap.scaled(size));
    after5->setPixmap(pixmap.scaled(size));

    layout->addWidget(after);
    layout->addWidget(after2);
    layout->addWidget(after3);
    layout->addWidget(after4);
    layout->addWidget(after5);
}
