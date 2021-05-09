#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QImageReader>
#include <QMimeData>

#include "threadpool.h"
#include "functions.h"

enum class Type_e
{
    LINEAR,
    MEDIAN,
    GAUSS
};

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void start();
    void dragEnterEvent(QDragEnterEvent *in_Event) override;
    void dropEvent(QDropEvent *in_Event) override;

private:
    QImage image;
    QLabel imageLabel;
    Type_e type;
    QTextEdit text;
    QLineEdit radius;
    QLineEdit sigma;
    ThreadPool<std::function<bool()>> pool;
};
#endif // FORM_H
