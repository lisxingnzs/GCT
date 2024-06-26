#ifndef DASHBOARD1_H
#define DASHBOARD1_H
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QtMath>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class Dashboard1;
}

class Dashboard1 : public QWidget
{
    Q_OBJECT
public:
    explicit Dashboard1(QWidget *parent = nullptr);
    ~Dashboard1();
    void setValue(int qValue);
    double value();
    void setMaxValue(float fMaxValue);//设置表盘最大的值
    void setDashBoardEdgeColor(QColor &qColor);//设置边缘的颜色
    void setDashDivideBackgroudColor(QColor &qColor);//设置表盘中央的颜色
    void MovetoValue(int qValue1);
signals:
    void valueChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
private:
    void qParmInit();//全局参数初始化
    //鼠标释放时检测窗口大小是否一样，不一样则自动设置为一样
    void setOtherBackgroud(bool status,QPainter *painter);
    void drawDashBoardWiget(QPainter *painter);
    void drawDashBoard(QPainter *painter);//绘制表盘
    void drawDivideRule(QPainter *painter);//绘制刻度
    void drawDashBoardText(QPainter *painter);//绘制表盘文字
    void drawDishBoardPointer(QPainter *painter);//绘制指针
    void timework();
    int calculateValueFromMousePosition(QPoint point);
    int fMaxValue = 120;
    int qValue = 0;
    int moveToValue;
    int fMaxDivide = fMaxValue/10;//大等分
    int fMinDivide = 5;
    float uiOldWidth;
    float uiOldHeight;
    QPoint qCircular;           //圆形位置
    int  qRadius = 0;           //半径
    QColor qDialEdgeColor = QColor(80,80,80);
    int    qDialEdgeSize = 0;                                     ///*外部表盘
    QColor qDialDivideBackgroudColor = QColor(20,20,20);//表盘刻度背景颜色
    int    qDialDivideSize = 0;//刻度盘大小，是外部表盘的1.5倍
    float BaseAngle = 135;//基础角度
    QTimer *PaintTimer;//定时刷新
    QTimer *MoveTimer;
    bool isDragging;
    QPoint lastPos;
    Ui::Dashboard1 *ui;
};

#endif // DASHBOARD1_H
