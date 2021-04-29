#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMimeData>
#include <QImageReader>
#include <QDragEnterEvent>
#include <QErrorMessage>

#define IMAGE_WIDTH 920
#define IMAGE_HEIGHT 800

#include "threadpool.h"

struct Pixel_s
{
    uchar r;
    uchar g;
    uchar b;
};

enum class Algorithms_e
{
    NONE
};

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void dragEnterEvent(QDragEnterEvent *in_Event) override;
    void dropEvent(QDropEvent *in_Event) override;
    void start();

private:
    void calculate(Pixel_s* out_NewImage);
    void checkCountParams(const QStringList& in_List, int in_Needed) const

private:
    QImage m_Image;
    QLabel m_ImageLabel;
    QLineEdit m_InputData;
    QComboBox m_InputAlgorithm;
    ThreadPool m_Pool;
};
#endif // FORM_H
