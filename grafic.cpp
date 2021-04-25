#include "grafic.h"

Graphic::Graphic(QWidget *parent)
    : QGraphicsScene(parent)
{
    Form *form = (Form*)parent;

    connect(this, SIGNAL(GraphicChanged(const std::array<int, 256>&)), form, SLOT(UpdatePicture(const std::array<int, 256>&)));

    for (std::size_t i = 0; i < m_Values.size(); ++i)
        m_Values[i] = i;

    m_GraficPoints[0] = 0;
    m_GraficPoints[255] = 255;

    drawGraphic();
}

void Graphic::mousePressEvent(QGraphicsSceneMouseEvent *in_Event)
{
    QPointF mousePoint = in_Event->scenePos();
    int x = 256. / CHANGED_GRAPHIC_SIZE * (mousePoint.x() + 50 - POINT_SIZE);
    int y = 256. / CHANGED_GRAPHIC_SIZE * (CHANGED_GRAPHIC_SIZE - (mousePoint.y() + 50 - POINT_SIZE));
    bool canDelete = false;
    QPoint resultPoint = getExistPoint(x, canDelete);

    if (resultPoint.x() != -1)
        x = resultPoint.x();

    switch (in_Event->button())
    {
        case Qt::MouseButton::LeftButton:
        {
            m_GraficPoints[x] = y;
        } break;
        case Qt::MouseButton::RightButton:
        {
            if (canDelete)
            {
                m_GraficPoints.erase(m_GraficPoints.find(x));
            }
        } break;

        default:
            return;
    }

    calculate();

    emit GraphicChanged(m_Values);

    drawGraphic();
}

QPoint Graphic::getExistPoint(int in_X, bool &out_CanBeDeleted) const
{
    for (int i = -POINT_SIZE; i < POINT_SIZE; ++i)
    {
        int x = in_X + i;
        if (m_GraficPoints.count(x))
        {
            out_CanBeDeleted = !(!x || x == 255); //if x == 0 or x == 255 we can`t delete point from graphic
            return {x, m_GraficPoints.find(x)->second};
        }
    }
    return {-1, -1};
}

void Graphic::calculate()
{
    auto it = m_GraficPoints.begin();
    double B = 0;

    auto null_index = it;
    auto first_index = ++it;
    double h = first_index->first - null_index->first;
    double f = (first_index->second - null_index->second) / h;
    double a = 1 / std::pow(h, 2) * (first_index->second - null_index->second) - (1. / h) * f;
    double b = f - 2 * a * null_index->first;
    double c = null_index->second - f * null_index->first + a * null_index->first * null_index->first;

    auto CalculateSpline = [&](double h, double a, double b, double c, const std::map<int, int>::iterator&  null_index) {
        for (int j = 0; j <= h; j ++)
        {
            int X = null_index->first + j;
            int Y = (int)(a * X * X + b * X + c);
            X = X > 255 ? 255 : X;
            X = X < 0 ? 0 : X;
            Y = Y > 255 ? 255 : Y;
            Y = Y < 0 ? 0 : Y;
            m_Values[X] = Y;
        }
    };
    CalculateSpline(h, a, b, c, null_index);

    for (; it != m_GraficPoints.end(); ++it)
    {
        if (++it != m_GraficPoints.end())
            --it;
        else
            return;
        auto tmp_index = it;
        auto second_index = ++it;
        --it;
        h = second_index->first - tmp_index->first;
        B = 2 * a * tmp_index->first + b;
        a = 1 / std::pow(h, 2) * (second_index->second - tmp_index->second) - (1 / h) * B;
        b = B - 2 * a * tmp_index->first;
        c = tmp_index->second - B * tmp_index->first + a * tmp_index->first * tmp_index->first;

        CalculateSpline(h, a, b, c, tmp_index);
    }
}



void Graphic::drawGraphic()
{
    clear();

    QPen pen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    for (const auto& point : m_GraficPoints)
    {
        int x = CHANGED_GRAPHIC_SIZE / 256. * point.first;
        int y = CHANGED_GRAPHIC_SIZE / 256. * point.second;
        x -= 50;
        y = CHANGED_GRAPHIC_SIZE - y - 50;
        addEllipse(x, y, POINT_SIZE * 2, POINT_SIZE * 2, pen);
    }

    int x = CHANGED_GRAPHIC_SIZE / 256. * 0;
    int y = CHANGED_GRAPHIC_SIZE / 256. * m_Values[0];
    x -= 50;
    y = CHANGED_GRAPHIC_SIZE - y - 50;
    QPainterPath path({static_cast<float>(x), static_cast<float>(y)});
    for (std::size_t i = 1; i < m_Values.size(); ++i)
    {
        x = CHANGED_GRAPHIC_SIZE / 256. * i;
        y = CHANGED_GRAPHIC_SIZE / 256. * m_Values[i];
        x -= 50;
        y = CHANGED_GRAPHIC_SIZE - y - 50;
        path.lineTo(x, y);
    }

    QBrush brush(Qt::GlobalColor::green);
    pen.setBrush(brush);
    addPath(path, pen);

    update();
}









