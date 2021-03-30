#include "grafic.h"

class From;

Grafic::Grafic(QWidget *parent) : QWidget(parent)
{
    From *form = (From*)parent;
}
