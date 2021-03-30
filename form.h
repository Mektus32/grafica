#ifndef FORM_H
#define FORM_H

#include <list>

#include <QWidget>
#include <QPicture>
#include <QChart>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QChartView>
#include <QGraphicsView>
#include <QMimeData>

#include "grafic.h"

class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();
    void UpdatePicture(QPoint in_Point);

private slots:
    void dragEnterEvent(QDragEnterEvent *in_Event) override;
    void dropEvent(QDropEvent *in_Event) override;

private:
    QtCharts::QChart m_InitialHistogram;
    QtCharts::QChart m_UpdatedHistogram;
    std::list<QPoint> m_GraficPoints;
    QLabel m_LabelWithImage;
    QImage m_ResultImage;
};
#endif // FORM_H
