#ifndef SPEEDPANEL_H
#define SPEEDPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QtMath>
#include <QPainterPath>
#include <QDebug>
#include <QRadialGradient>//渐变
#include <QTimer>
#include <QFontMetricsF>
#define YELLOW QColor(Qt::yellow)
#define WHITE QColor(Qt::white)
#define RED QColor(Qt::red)
#define GREEN QColor(Qt::green)
#define BLACK QColor(Qt::black)


class SpeedPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ getValue WRITE setValue)  //声明属性
public:
    explicit SpeedPanel(QWidget *parent = nullptr);
    void setValue(qreal value);
    qreal getValue();


public slots:
    void speedUpdated();//速度变化槽函数


private:

    void DrawSmallScale(QPainter& painter,int radius);//画刻度线
    void DrawCircle(QPainter& painter,int radius);//渐变发光外扇形
    void DrawUnit(QPainter& painter,int radius);//单位和标题显示
    void DrawPointer(QPainter& painter,int radius);//指针
    void DrawCircle_line(QPainter& painter,int radius);//最外圆细线
    void DrawCircle_bom_big(QPainter& painter,int radius);//中间大圆
    void DrawCircle_bom_shine(QPainter& painter,int radius);//渐变发光内圈
    void DrawCircle_bom_small(QPainter& painter,int radius);//中间小圆
    void DrawCircle_arc(QPainter& painter,int radius);//动态扇形环.

private:
    QPixmap m_bg;//背景图片
    QTimer *myTimer;
    int radius;//仪表盘的中心位置
    int direction;//指针运动的方向,1为前进，0为后退
    int degRotate=0;
    int flag=1;//0后退，1前进


protected:
    void paintEvent(QPaintEvent *event) override;


signals:

};

#endif // SPEEDPANEL_H
