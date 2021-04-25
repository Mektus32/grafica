#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "form.h"

#include <map>
#include <array>
#include <cmath>

#define POINT_SIZE 3
#define CHANGED_GRAPHIC_SIZE (GRAPHIC_SIZE - POINT_SIZE * 2)

class Graphic : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Graphic(QWidget *parent);

signals:
    void GraphicChanged(const std::array<int, 256>& ref_Values);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *in_Event) override;
    QPoint getExistPoint(int in_X, bool& out_CanBeDeleted) const;
    void calculate();
    void drawGraphic();

private:
    std::array<int, 256> m_Values;
    std::map<int, int> m_GraficPoints;
};

#endif // GRAPHIC_H
