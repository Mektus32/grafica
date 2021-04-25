#include <QApplication>

#include "form.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form w;

    w.setFixedSize(1349, 822);

    w.show();
    return a.exec();
}
