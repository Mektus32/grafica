#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollArea>

#include "form.h"

form::form(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *hbox = new QHBoxLayout(this);
    QVBoxLayout *vbox = new QVBoxLayout();

//    QPixmap pixmap("1.jpg");
//    QLabel *label = new QLabel("here", this);

//    label->setPixmap(pixmap.scaled(500, 500));

//    QGraphicsScene *scene = new QGraphicsScene(this);
//    scene->addPixmap(pixmap);
//    QGraphicsView *view = new QGraphicsView(scene);

//    QPushButton *button1 = new QPushButton("but1");
//    QPushButton *button2 = new QPushButton("but2");
//    QPushButton *button3 = new QPushButton("but3");
//    //layer *lay = new layer();

    QScrollArea *area = new QScrollArea();
    PictureSettings *pic = new PictureSettings("1.jpg");
    area->setWidget(pic);
//    //area->setWidget(lay);

//    hbox->addWidget(label);
    vbox->addWidget(area);
//    vbox->addWidget(button3);

    hbox->addLayout(vbox);
}
