#include "plottertest.h"
#include "plotterwidget.h"

PlotterTest::PlotterTest(QWidget *parent) : QDialog(parent)
{

    PlotterWidget *widget = new PlotterWidget(this);
    this->resize(QSize(widget->size()));
}
