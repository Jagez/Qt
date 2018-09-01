#include "plotterwidget.h"
#include <QtGui>
#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <cmath>
using namespace std;


void PlotterWidget::init()
{
    m_isRubberBandShown = false;
    m_btnZoomIn = new QToolButton(this);
    m_btnZoomIn->setIcon(QIcon(":/icon/icon/zoomIn.png"));
    m_btnZoomIn->adjustSize();
    connect(m_btnZoomIn, &QToolButton::clicked, this, &PlotterWidget::zoomIn);

    m_btnZoomOut = new QToolButton(this);
    m_btnZoomOut->setIcon(QIcon(":/icon/icon/zoomOut.png"));
    m_btnZoomOut->adjustSize();
    connect(m_btnZoomOut, &QToolButton::clicked, this, &PlotterWidget::zoomOut);

    this->setPlotSettings(PlotSettings());
}

PlotterWidget::PlotterWidget(QWidget *parent) : QWidget(parent)
{
    this->setBackgroundRole(QPalette::Dark);
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::StrongFocus);
    this->init();

}

void PlotterWidget::setPlotSettings(const PlotSettings &settings)
{
    m_vecZoomStack.clear();
    m_vecZoomStack.append(settings);
    m_curZoom = 0;
    m_btnZoomIn->hide();
    m_btnZoomOut->hide();
    this->refreshPixmap();
}

void PlotterWidget::setCurveData(int id, const QVector<QPointF> &data)
{
    m_mapCurve[id] = data;
    this->refreshPixmap();
}

void PlotterWidget::clearCurve(int id)
{
    m_mapCurve.remove(id);
    this->refreshPixmap();
}

QSize PlotterWidget::minimumSizeHint() const
{
    return QSize(6 * Margin, 8 * Margin);
}

QSize PlotterWidget::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void PlotterWidget::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, m_pixMap);
    if (true == m_isRubberBandShown)
    {
        painter.setPen(palette().light().color());
        painter.drawRect(m_rectRubberBand.normalized().adjusted(0, 0, -1, -1));
    }
    if (hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void PlotterWidget::resizeEvent(QResizeEvent *event)
{
    int x = this->width() - (m_btnZoomIn->width() + m_btnZoomOut->width() + 10);
    m_btnZoomIn->move(x, 5);
    m_btnZoomOut->move(x + m_btnZoomIn->width() + 5, 5);
    this->refreshPixmap();
}

void PlotterWidget::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (event->button() == Qt::LeftButton)
    {
        if (rect.contains(event->pos()))
        {
            m_isRubberBandShown = true;
            m_rectRubberBand.setTopLeft(event->pos());
            m_rectRubberBand.setBottomRight(event->pos());
            this->updateRubberBandRegion();
            this->setCursor(Qt::CrossCursor);
        }
    }
}

void PlotterWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && (m_isRubberBandShown == true))
    {
        m_isRubberBandShown = false;
        this->updateRubberBandRegion();
        this->unsetCursor();
        QRect rect = m_rectRubberBand.normalized();
        if (rect.width() < 4 || rect.height() < 4)
        {
            return;
        }
        rect.translate(-Margin, -Margin);
        PlotSettings prevSettings = m_vecZoomStack[m_curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.m_minX = prevSettings.m_minX + dx * rect.left();
        settings.m_maxX = prevSettings.m_minX + dx * rect.right();
        settings.m_minY = prevSettings.m_maxY - dy * rect.bottom();
        settings.m_maxY = prevSettings.m_maxY - dy * rect.top();
        settings.adjust();
        m_vecZoomStack.resize(m_curZoom + 1);
        m_vecZoomStack.append(settings);
        this->zoomIn();
    }
}

void PlotterWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (true == m_isRubberBandShown)
    {
        this->updateRubberBandRegion();
        m_rectRubberBand.setBottomRight(event->pos());
        this->updateRubberBandRegion();
    }
}

void PlotterWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        this->zoomIn();
        break;
    case Qt::Key_Minus:
        this->zoomOut();
        break;
    case Qt::Key_Left:
        m_vecZoomStack[m_curZoom].scroll(-1, 0);
        this->refreshPixmap();
        break;
    case Qt::Key_Right:
        m_vecZoomStack[m_curZoom].scroll(+1, 0);
        this->refreshPixmap();
        break;
    case Qt::Key_Down:
        m_vecZoomStack[m_curZoom].scroll(0, -1);
        this->refreshPixmap();
        break;
    case Qt::Key_Up:
        m_vecZoomStack[m_curZoom].scroll(0, +1);
        this->refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void PlotterWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;
    if (event->orientation() == Qt::Horizontal)
    {
        m_vecZoomStack[m_curZoom].scroll(numTicks, 0);
    }
    else
    {
        m_vecZoomStack[m_curZoom].scroll(0, numTicks);
    }
    this->refreshPixmap();
}


void PlotterWidget::updateRubberBandRegion()
{
    QRect rect = m_rectRubberBand.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void PlotterWidget::refreshPixmap()
{
    m_pixMap = QPixmap(this->size());
    m_pixMap.fill(this, 0, 0);
    QPainter painter(&m_pixMap);
    painter.initFrom(this);
    this->drawGrid(&painter);
    this->drawCurves(&painter);
    update();
}

void PlotterWidget::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (false == rect.isValid())
    {
        return;
    }
    PlotSettings settings = m_vecZoomStack[m_curZoom];
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();
    int i = 0;
    for (i; i <= settings.m_numXTicks; ++i)
    {
        int x = rect.left() + (i * (rect.width() -1) / settings.m_numXTicks);
        double label = settings.m_minX + (i * settings.spanX() / settings.m_numXTicks);
        painter->setPen((quiteDark));
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
    int j = 0;
    for (j; j <= settings.m_numYTicks; ++j)
    {
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.m_numYTicks);
        double label = settings.m_minY + (j * settings.spanY() / settings.m_numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void PlotterWidget::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] =
    {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };
    PlotSettings settings = m_vecZoomStack[m_curZoom];
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (false == rect.isValid())
    {
        return;
    }
    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    QMapIterator<int, QVector<QPointF> > i(m_mapCurve);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        const QVector<QPointF> &data = i.value();
        QPolygonF polyline(data.count());
        int j = 0;
        for (j; j < data.count(); ++j)
        {
            double dx = data[j].x() - settings.m_minX;
            double dy = data[j].y() - settings.m_minY;
            double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
            polyline[j] = QPointF(x, y);
        }
        painter->setPen(colorForIds[uint(id) % 6]);
        painter->drawPolyline(polyline);
    }
}

void PlotterWidget::zoomIn()
{
    if (m_curZoom < m_vecZoomStack.count() - 1)
    {
        ++m_curZoom;
        m_btnZoomIn->setEnabled(m_curZoom < m_vecZoomStack.count() - 1);
        m_btnZoomOut->setEnabled(true);
        m_btnZoomOut->show();
        this->refreshPixmap();
    }
}

void PlotterWidget::zoomOut()
{
    if (m_curZoom > 0)
    {
        --m_curZoom;
        m_btnZoomOut->setEnabled(m_curZoom > 0 ? 1 : 0);
        m_btnZoomIn->setEnabled(true);
        m_btnZoomOut->show();
        this->refreshPixmap();
    }
}

PlotSettings::PlotSettings()
{
    this->m_minX = 0.0;
    this->m_maxX = 10.0;
    this->m_numXTicks = 5;
    this->m_numYTicks = 5;
    this->m_minX = 0.0;
    this->m_maxY = 10.0;
}

void PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX() / m_numXTicks;
    m_minX += dx * stepX;
    m_maxX += dx * stepX;
    double stepY = spanY() / m_numYTicks;
    m_minY += dy * stepY;
    m_maxY += dy * stepY;
}

void PlotSettings::adjust()
{
    adjustAxis(m_minX, m_maxX, m_numXTicks);
    adjustAxis(m_minY, m_maxY, m_numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep= (max - min) / MinTicks;
    double step = pow(10.0, floor(log10(grossStep)));
    if (5 * step < grossStep)
    {
        step *= 5;
    }
    else if (2 * step < grossStep)
    {
        step *= 2;
    }
    numTicks = int(ceil(max / step) - floor(min / step));
    if (numTicks < MinTicks)
    {
        numTicks = MinTicks;
    }
    min = floor(min / step) * step;
    max = ceil(max / step) * step;
}
