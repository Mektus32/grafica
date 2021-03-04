#ifndef LAYER_H
#define LAYER_H

#include <QWidget>
#include <QPixmap>

enum class Operation_e {
    SUM
};

class Layer : public QWidget
{
    Q_OBJECT
public:
    explicit Layer(QWidget *parent = nullptr);

signals:

private:
    QPixmap m_Pixmap;


};

#endif // LAYER_H
