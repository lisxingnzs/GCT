#include "dashboard1.h"
//#include "ui_dashboard1.h"
#include "QDebug"
Dashboard1::Dashboard1(QWidget *parent) :
    QWidget(parent)
{
    PaintTimer = new QTimer(this);
    MoveTimer = new QTimer(this);
    connect(PaintTimer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&Dashboard1::update));
    connect(MoveTimer,&QTimer::timeout,this,&Dashboard1::timework);
    PaintTimer->start(50);
    qDialEdgeColor =QColor(80,80,80);
    setDashBoardEdgeColor(qDialEdgeColor);
}

Dashboard1::~Dashboard1()
{
}

void Dashboard1::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    this->drawDashBoardWiget(&painter);
    //QWidget::paintEvent(event); // 调用父类的paintEvent函数，以便处理鼠标事件
}


//指针缓冲运动
void Dashboard1::timework()
{
    if(qValue == moveToValue)
    {
        qDebug()<<qValue;
         MoveTimer->stop();
         qValue = moveToValue;
    }
    else
    {
        int dis = this->qValue - moveToValue;
        if(dis< 0)
        {   qValue+= (-dis)/2+1;
        }
        else if(dis> 0)
        {
            qValue-= (dis/2)+1;

        }
    }
}
void Dashboard1::qParmInit()
{
    qCircular = QPoint(width() / 2,height() / 2);//圆心位置刷新
    qRadius = width()>height()?height() / 2:width()/2;//外圆形的半径
    qDialEdgeSize=qRadius*0.1;
    qDialDivideSize=qDialEdgeSize*1.5;
}


void Dashboard1::setOtherBackgroud(bool status,QPainter *painter)
{
    if(status == true)
    {
        painter->save();

        painter->setBrush(Qt::transparent);
        painter->fillRect(this->rect(), QColor(0, 0, 0, 0));
        painter->restore();
    }
}
//绘制控件
void Dashboard1::drawDashBoardWiget(QPainter *painter)
{
    //全局参数初始化
    qParmInit();
    setOtherBackgroud(true,painter);
    //绘制表盘
    drawDashBoard(painter);
    //绘制刻度尺
    drawDivideRule(painter);
    //绘制指针
    drawDishBoardPointer(painter);
    //绘制表盘文字
    drawDashBoardText(painter);
}

//绘制仪表盘
void Dashboard1::drawDashBoard(QPainter *painter)
{
    painter->save();

    painter->setBrush(Qt::transparent);
    painter->fillRect(this->rect(), QColor(0, 0, 0, 0));


    QBrush qBrush = QBrush(qDialEdgeColor);//灰色画刷,设置表盘边缘原色

    painter->setBrush(qBrush);//使用画刷

    painter->drawEllipse(qCircular,qRadius,qRadius);        //绘制最外层圆盘

    qBrush.setColor(qDialDivideBackgroudColor);//黑色画刷

    painter->setBrush(qBrush);

    painter->drawEllipse(qCircular,qRadius - qDialEdgeSize,qRadius - qDialEdgeSize);//绘制黑色条带

    qBrush.setColor(qDialEdgeColor);//灰色画刷
    painter->setBrush(qBrush);
    painter->drawEllipse(qCircular,qRadius - qDialDivideSize- qDialEdgeSize,
                         qRadius - qDialDivideSize- qDialEdgeSize);//绘制内层灰色圆盘

    qBrush =  QColor(162,88,88);//粉色画刷
    painter->setBrush(qBrush);
    int tem=qRadius*0.2;
    painter->drawEllipse(qCircular,tem,tem);//内部最大粉色圆盘

    painter->restore();
}

void Dashboard1::drawDivideRule(QPainter *painter)
{
    painter->save();

    //下面开始绘制刻度，90度的间隔
    //x2 + y2 = r2
    //则圆上任一点为：（x1,y1）
    //x1   =   x0   +   r   *   cos(ao   *   3.14   /180   )
    //y1   =   y0   +   r   *   sin(ao   *   3.14   /180   )


    float fRadius = 0.0;
    //为了对称好看我们一般从逆时针90度位置开始绘制

    QPen qPen(Qt::white);
    qPen.setWidth(2);   //设置画笔的粗细
    painter->setPen(qPen);
    //先绘制左侧
    for(int uiLoop = 0;uiLoop < (fMaxDivide * fMinDivide) + 1 ;uiLoop++)
    {
        float fAngle = (BaseAngle + ((float)270 / (fMaxDivide * fMinDivide)) * uiLoop);

        fRadius = (qRadius - qDialEdgeSize);

        int x1Start = qCircular.x() + fRadius * cos(fAngle * 3.14 / 180);
        int y1Start = qCircular.y() + fRadius * sin(fAngle * 3.14 / 180);

        if(uiLoop % fMinDivide == 0)
        {

            fRadius = qRadius - qDialDivideSize  - qDialEdgeSize;
        }
        else
        {
            fRadius = qRadius - qDialDivideSize  - qDialEdgeSize*0.2;
        }
        int x2End = qCircular.x() + fRadius * cos(fAngle * 3.14 / 180);
        int y2End =  qCircular.y() + fRadius * sin(fAngle * 3.14 / 180);

        painter->drawLine(QPoint(x1Start,y1Start),QPoint(x2End,y2End));
    }
    painter->restore();
}

void Dashboard1::drawDashBoardText(QPainter *painter)
{
    painter->save();

    QPen qPen(Qt::white);
    qPen.setWidth(2);   //设置画笔的粗细
    painter->setPen(qPen);

    //绘制表盘文字
    QFont qFont("楷体",28,QFont::Bold,false);
    painter->setFont(qFont);//

    QFontMetrics fm(qFont); // 使用相同的字体来计算文本的尺寸
    int textWidth = fm.width(QString::number(qValue)); // 计算文本的宽度
    int textHeight = fm.height(); // 计算文本的高度

    // 动态计算矩形区域的位置和大小
    QRectF textRect(qCircular.x() - textWidth / 2, qCircular.y() + textHeight / 4, textWidth, textHeight);
    painter->drawText(textRect, Qt::AlignCenter, QString::number(qValue)); // 在动态计算的矩形区域内绘制文本
    float fRadius = 0.0;
    //通过公式计算刻度文字的位置信息。
    for(int uiLoop = 0;uiLoop < (fMaxDivide ) + 1 ;uiLoop++)
    {
        int PointeNum = ((fMaxValue / fMaxDivide) * uiLoop);        //刻度值

        float fAngle = (BaseAngle + ((float)270 / (fMaxDivide)) * uiLoop);

        fRadius = (qRadius - qDialEdgeSize-qDialDivideSize) ;

        int x1Start = qCircular.x() + fRadius * cos(fAngle * 3.14 / 180);
        int y1Start = qCircular.y() + fRadius * sin(fAngle * 3.14 / 180);

        QFont qFonts("楷体",12,QFont::Bold,false);
        painter->setFont(qFonts);

        QFontMetrics fm(qFont); // 使用相同的字体来计算文本的尺寸
        int textWidth = fm.width(QString::number(PointeNum)); // 计算文本的宽度
        int textHeight = fm.height(); // 计算文本的高度
        QRectF textRect(x1Start - textWidth / 2, y1Start - textHeight / 2, textWidth, textHeight);
        painter->drawText(textRect, Qt::AlignCenter, QString::number(PointeNum));
    }
    painter->restore();
}
//绘制指针
void Dashboard1::drawDishBoardPointer(QPainter *painter)
{
    painter->save();

    QBrush qBrush = QBrush(QColor(Qt::red));

    painter->setBrush(qBrush);


    QPen qPen(Qt::red);
    qPen.setWidth(2);   //设置画笔的粗细
    painter->setPen(qPen);

    float fRadius = 0.0;
    if(qValue > fMaxValue)
    {
        qValue = fMaxValue;
    }

    float fAngle = (BaseAngle + ((float)270 / (fMaxValue)) * qValue);//指针中心角度信息。
    float fRightAngle = fAngle + 90;//右侧角度
    float fLeftAngle = fAngle - 90;//左侧角度

    fRadius = (qRadius - qDialEdgeSize- qDialDivideSize);

    int x1Start = qCircular.x() + fRadius * cos(fAngle * 3.14 / 180);
    int y1Start = qCircular.y() + fRadius* sin(fAngle * 3.14 / 180);

    int tem2=qRadius*0.075;
    int x3End = qCircular.x() + tem2 * cos(fRightAngle * 3.14 / 180);//过圆心的右侧切点
    int y3End = qCircular.y() + tem2 * sin(fRightAngle * 3.14 / 180);

    int x4End = qCircular.x() + tem2 * cos(fLeftAngle * 3.14 / 180);//过圆心的左侧切点
    int y4End = qCircular.y() + tem2 * sin(fLeftAngle * 3.14 / 180);


    QPointF qTriangle[3] = {QPoint(x1Start,y1Start),QPoint(x3End,y3End),QPoint(x4End,y4End)};

    painter->drawPolygon(qTriangle,3);
    int tem=qRadius*0.1;
    painter->drawEllipse(qCircular,tem,tem);//内部最大粉色圆盘
    painter->restore();
}

void Dashboard1::setValue(int qValue)
{
    this->qValue = qValue;
}

double Dashboard1::value()
{
    return this->qValue;
}
//动态移动到对应值
void Dashboard1::MovetoValue(int qValue1)
{
    MoveTimer->start(50);
    moveToValue = qValue1;
}


void Dashboard1::setMaxValue(float fMaxValue)
{
    this->fMaxValue = fMaxValue;
}

void Dashboard1::setDashBoardEdgeColor(QColor &qColor)
{
    qDialEdgeColor = qColor;
}

void Dashboard1::setDashDivideBackgroudColor(QColor &qColor)
{
    qDialDivideBackgroudColor = qColor;
}

void Dashboard1::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        int newValue = calculateValueFromMousePosition(lastPos);
        setValue(newValue);  // 更新表盘值
        emit valueChanged(newValue);
    }
}

void Dashboard1::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        int newValue = calculateValueFromMousePosition(event->pos());
        setValue(newValue);  // 更新表盘值
        emit valueChanged(newValue);
    }
}

void Dashboard1::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

int Dashboard1::calculateValueFromMousePosition(QPoint point)
{
    int dx=point.x()-qCircular.x();
    int dy=point.y()-qCircular.y();
    double angle = atan2(dy, dx) * 180 / M_PI;
    // 将负角度转换为正角度
    if (angle < 0) {
        angle += 360;
    }
    // 将角度限制在0到360度之间
    angle = fmod(angle, 360);
    // 将角度限制在0到360度之间
    angle = fmod(angle, 360);

    // 计算从135度到40度之间的角度范围
    double startAngle = 135;
    double endAngle = 45;
    double mappedAngle = 0;

    if (angle >= startAngle) {
        mappedAngle = angle - startAngle;
    } else {
        mappedAngle = 360 - (startAngle - angle);
    }
    int newValue=0;
    if(mappedAngle<=270)
        newValue=mappedAngle/270*fMaxValue;
    else if(mappedAngle<315)
        newValue=fMaxValue;
    else
        newValue=0;
    return newValue;

}

