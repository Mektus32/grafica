#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QScrollArea>

#include "form.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    Form window;

    window.setFixedSize(1400, 768);
    window.setWindowTitle("Simple example");
    window.show();

    return app.exec();
}
