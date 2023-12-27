#include "thermometredlg.h"

ThermometreDlg::ThermometreDlg(QWidget *parent) : QWidget(parent)
{
    m_width = 20;
    maxValue = 100;
    minValue = 0;
    m_radius = 1.05;
    m_value = 0;
    curValue = m_value;
    QTimer *at = new QTimer(this);
    at->start(1000);
    m_valueAnimation = new QPropertyAnimation(this, "value");
    m_valueAnimation->setDuration(1000);
    m_valueAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_valueAnimation->setLoopCount(1);
    connect(at, SIGNAL(timeout()), this, SLOT(startAnimation()));

}

void ThermometreDlg::updateRect()
{
    m_rect.setX(0);
    m_rect.setY(20 - height()/2);
    m_rect.setWidth(m_width);
    m_rect.setHeight(height() - 40 - m_width* m_radius);
}

void ThermometreDlg::setValue(qreal value)
{
    m_value = value;
    update();
}

void ThermometreDlg::changeValue(qreal value)
{
    if(value > maxValue)
        value = maxValue;
    if(value < minValue)
        value = minValue;
    curValue = value;
}

qreal ThermometreDlg::getValue()
{
    return m_value;
}


void ThermometreDlg::paintEvent(QPaintEvent *e)
{
    updateRect();
    QPainter painter(this);
    QPen pen(Qt::black);
    painter.translate(this->width()/2, this->height()/2);  //坐标轴移动到中心点
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);  // 启用反锯齿
    painter.save();
    //绘制上方的柱状
    painter.fillRect(m_rect, QColor(168,200, 225));

    //绘制底部的圆
    QRectF tmpRect = QRectF(m_rect.bottomLeft(), QPointF(m_width, height()/2- m_width*m_radius));
    painter.fillRect(tmpRect, QColor(255, 0, 0));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 0, 0));
    painter.drawEllipse(tmpRect.bottomLeft()+QPointF(tmpRect.width()/2, 0),m_width*m_radius, m_width*m_radius);
    painter.restore();

    //绘制刻度以及刻度值
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
    painter.fillRect(m_rect.adjusted(0, m_rect.height()-h-perHeight/2-1 , 0, 0), QColor(255, 0, 0));

    QWidget::paintEvent(e);
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
