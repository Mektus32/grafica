#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QImage>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QImageReader>
#include <QMimeData>


#include "threadpool.h"
#include "processing.h"


class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void calculate();
    void dragEnterEvent(QDragEnterEvent *in_Event) override;
    void dropEvent(QDropEvent *in_Event) override;

private:
    bool getData(QStringList& factors_data, QStringList& circle_data) const noexcept;


private:
    QLabel  imageLabel;
    QLabel  fourier;
    QImage  image;
    QLineEdit   factors;
    QTextEdit   circle;
    ThreadPool pool;
};
#endif // FORM_H
