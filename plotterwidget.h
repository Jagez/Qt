#ifndef PLOTTERWIDGET_H
#define PLOTTERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>

//Rubber band（橡皮筋线，或者橡皮线），
//pixmap（图像，双缓冲中用到的图像，有时也直 呼 pixmap），
//off-screen pixmap（离线图像
class QToolButton;
class PlotSettings;

class PlotterWidget : public QWidget
{
    Q_OBJECT
private:
    void init();

public:
    explicit PlotterWidget(QWidget *parent = nullptr);
    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    //重写QWidget的两个函数
    QSize minimumSizeHint() const;
    QSize sizeHint() const;


protected:
    //重写事件处理函数
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    //The QWheelEvent class contains parameters that describe a wheel event.
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

private:
    enum { Margin = 50 };
    QToolButton *m_btnZoomIn;
    QToolButton *m_btnZoomOut;
    QMap<int, QVector<QPointF>> m_mapCurve;           //曲线数据
    QVector<PlotSettings> m_vecZoomStack;
    int m_curZoom;
    bool m_isRubberBandShown;
    QRect m_rectRubberBand;
    QPixmap m_pixMap;                                //显示在屏幕的控件的一个拷贝

public slots:
    void zoomIn();                  //放大曲线
    void zoomOut();                 //缩小显示区

signals:

public slots:
};


//PlotSettings 确定x，y轴的范围和刻度的个数
class PlotSettings
{
public:
    double m_minX;
    double m_maxX;
    double m_minY;
    double m_maxY;
    int m_numXTicks;
    int m_numYTicks;

public:
    PlotSettings();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return m_maxX - m_minX; }
    double spanY() const { return m_maxY - m_minY; }

private:
    static void adjustAxis(double &min, double &max, int &numTicks);


};

#endif // PLOTTERWIDGET_H
