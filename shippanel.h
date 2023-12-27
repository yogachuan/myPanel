#ifndef SHIPPANEL_H
#define SHIPPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QtMath>
#include <QPainterPath>
#include <QDebug>
#include <QRadialGradient>//径向渐变
#include <QLinearGradient>//线性渐变
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QFontMetricsF>
#include <QPointF>



class ShipPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ShipPanel(QWidget *parent = nullptr);
    void degUpdate();//舵角变化槽函数
    void pitchUpdate();//俯仰变化槽函数

public:




private:

    void DrawHScale(QPainter& painter,int radius);//横摇刻度线
    void DrawCircle(QPainter& painter,int radius);//渐变发光外扇形
    void DrawUnit(QPainter& painter,int radius);//单位和标题显示
    void DrawPointer(QPainter& painter,int radius);//指针
    void DrawCircle_line(QPainter& painter,int radius);//最外圆细线
    void DrawVScale(QPainter& painter,int d);//纵摇刻度线
    void DrawBG(QPainter& painter,int l, int r);//画背景
    void DrawBaseLines(QPainter& painter);//画基准线


private:
    int radius;//仪表盘的中心位置
    int centerL;//窗口的左中心点
    int centerR;//窗口的右中心点
    QTimer *myTimer;
    int HDirection=0;//水平方向,1右,0左
    int VDirection=0;//竖直方向,1上,0下


    int degRotate =30;//角度,默认30在中间
    int pitch=0;//俯仰,默认0在中间

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // SHIPPANEL_H
