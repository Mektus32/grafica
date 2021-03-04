#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QScrollArea>

#include "form.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    form window;

    window.resize(250, 150);
    window.setWindowTitle("Simple example");
    window.show();

    return app.exec();
}
