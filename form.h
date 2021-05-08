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

#include "threadpool.h"
#include "algos.h"

struct ThreadData_s
{
    std::size_t m_Start;
    std::size_t m_Len;
    ThreadData_s(std::size_t in_Start, std::size_t in_Len)
        : m_Start(in_Start), m_Len(in_Len)
    {
    }
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
    void calculate(Pixel_s* out_NewImage, int in_Len, int in_A, double in_K, Algorithms_e in_Algo);
    bool checkCountParams(const QStringList& in_List, int in_Needed) const;

private:
    QImage m_Image;
    QLabel m_ImageLabel;
    QLineEdit m_InputData;
    QComboBox m_InputAlgorithm;
    std::vector<ThreadData_s> m_ThreadsData;
    ThreadPool<std::function<uint64_t()>> m_Pool;
};
#endif // FORM_H
