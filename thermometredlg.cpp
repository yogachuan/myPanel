#include "thermometredlg.h"

ThermometreDlg::ThermometreDlg(QWidget *parent) : QWidget(parent)
{
    m_width = 20;
    maxValue = 40;
    minValue = -10;
    m_radius = 1.05;
    m_value = 0;
    curValue = m_value;
    QTimer *at = new QTimer(this);
    at->start(1000);
//    m_valueAnimation = new QPropertyAnimation(this, "value");//value和头文件中的声明属性一致
//    m_valueAnimation->setDuration(1000);
//    m_valueAnimation->setEasingCurve(QEasingCurve::OutCubic);
//    m_valueAnimation->setLoopCount(1);

    connect(at, &QTimer::timeout, this, [=]{
        valueUpdate(curValue);
    });
//    connect(at, SIGNAL(timeout()), this, SLOT(startAnimation()));

}

void ThermometreDlg::paintEvent(QPaintEvent *event)
{
    updateRect();
    QPainter painter(this);
    QPen pen(Qt::black);
    painter.translate(this->width()/2, this->height()/2);  //坐标轴移动到中心点
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);  // 启用反锯齿
    //绘制上方的柱状
    drawRect(painter);
    //绘制底部的圆
    drawBottom(painter);

    //绘制刻度以及刻度值
    drawScale(painter);

    QWidget::paintEvent(event);
}

void ThermometreDlg::updateRect()
{
    m_rect.setX(0);
    m_rect.setY(20 - height()/2);
    m_rect.setWidth(m_width);
    m_rect.setHeight(height() - 40 - m_width* m_radius);
}

void ThermometreDlg::drawRect(QPainter &painter)
{
    painter.save();
    painter.fillRect(m_rect, QColor(168,200, 225));
    painter.restore();
}

void ThermometreDlg::drawBottom(QPainter &painter)
{
    painter.save();
    QRectF tmpRect = QRectF(m_rect.bottomLeft(), QPointF(m_width, height()/2- m_width*m_radius));
    QColor color = m_value > 30? QColor(255, 0, 0) : QColor(Qt::green);
    painter.fillRect(tmpRect, color);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    QPointF centerPoint(tmpRect.bottomLeft()+QPointF(tmpRect.width()/2, 0));
    painter.drawEllipse(centerPoint,m_width*m_radius, m_width*m_radius);

    painter.setPen(Qt::black);
    QFontMetricsF fm = QFontMetricsF(painter.font());
    QString str = QString("%1℃").arg(m_value);
    int w= (int)fm.width(str);
    int h = (int)fm.height();
    painter.drawText(QPointF(centerPoint.x()-w/2,centerPoint.y()+h/2), str);

    painter.restore();
}

void ThermometreDlg::drawScale(QPainter &painter)
{
    painter.save();
    painter.setPen(QColor(Qt::black));
    int nYCount = (maxValue - minValue)/10+1;
    qreal perHeight = (m_rect.height())/nYCount;
    for (int i=0; i<nYCount; ++i) {
        QPointF basePoint = m_rect.bottomLeft() - QPointF(0, perHeight/2) - QPointF(0, perHeight*i);
        //左侧大刻度
        painter.drawLine(basePoint, basePoint+QPointF(-10, 0));
        for (int j=1; j<10; ++j) {
            if(i == nYCount -1)
                continue;
            painter.drawLine(basePoint-QPointF(0, perHeight/10*j),basePoint-QPointF(5, perHeight/10*j));
        }
        painter.drawText(basePoint+QPointF(-28, 4), QString("%1").arg(minValue+i*10));
        //右侧大刻度
        basePoint = m_rect.bottomRight() - QPointF(0, perHeight/2) - QPointF(0, perHeight*i);
        painter.drawLine(basePoint, basePoint+QPointF(10, 0));
        for (int j=1; j<10; ++j) {
            if(i == nYCount -1)
                continue;
            painter.drawLine(basePoint-QPointF(0, perHeight/10*j),basePoint-QPointF(-5, perHeight/10*j));
        }
    }
    painter.restore();
    //根据值填充m_rect
    qreal h = (m_value-minValue)/(maxValue-minValue)*(m_rect.height()-perHeight);
    if(h<0)
        h = 0;
    if(h > m_rect.height())
        h = m_rect.height();
    QColor color = m_value > 30? QColor(255, 0, 0) : QColor(Qt::green);
    painter.fillRect(m_rect.adjusted(0, m_rect.height()-h-perHeight/2-1 , 0, 0), color);
}



void ThermometreDlg::setValue(qreal value)
{
    m_value = value;
    update();
}

void ThermometreDlg::valueUpdate(qreal value)
{
    if(flag)
    {
        value++;
        if(value > maxValue)
            flag=0;
    }
    else
    {
        value--;
        if(value < minValue)
            flag=1;

    }
    curValue = value;

    //使用动画的话，下面省略
    m_value = value;
    update();
}

qreal ThermometreDlg::getValue()
{
    return m_value;
}


void ThermometreDlg::startAnimation()
{
    qreal startValue = getValue();
    m_valueAnimation->setKeyValueAt(0, startValue-1);
    m_valueAnimation->setKeyValueAt(0.5, curValue+1);
    m_valueAnimation->setKeyValueAt(1, curValue);
    m_valueAnimation->setStartValue(startValue-2);
    m_valueAnimation->start();
}
