#include "shippanel.h"

#include <QPoint>


ShipPanel::ShipPanel(QWidget *parent) : QWidget(parent)
{

//    setFixedSize(1280,800);

    myTimer = new QTimer(this);
    myTimer->start(100);
    connect(myTimer, &QTimer::timeout, this, [=]{
        degUpdate();
        pitchUpdate();
    });

}

void ShipPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int width=this->width();
    int height=this->height();//移动仪表盘的高度
    int radius=((width>height)?height:width)/2.0;//仪表盘的中心位置

    //启用反锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    DrawBG(painter, height/2,height/2);//画背景
    //移动画笔到中下方
    painter.translate(width/2,height/2);

    painter.rotate(degRotate-30);

    DrawBaseLines(painter);//画基准线
    DrawHScale(painter,radius*0.85);//刻度线及数字
    DrawCircle(painter,radius*0.9);//渐变发光外扇形
    DrawPointer(painter,radius*0.9);//指针
    DrawCircle_line(painter,radius*0.9); //最外细圆线
    DrawUnit(painter, radius*0.1);//单位和标题显示

    DrawVScale(painter, radius*0.1);//纵摇刻度线及数字


}


//绘制刻度
void ShipPanel::DrawHScale(QPainter& painter,int radius)
{
    //组装点的路径图
    QPainterPath pointPath_small;
    pointPath_small.moveTo(-2,-2);
    pointPath_small.lineTo(-2,2);
    pointPath_small.lineTo(2,2);
    pointPath_small.lineTo(2,-2);


    QPainterPath pointPath_big;
    pointPath_big.moveTo(-2,-2);
    pointPath_big.lineTo(-2,2);
    pointPath_big.lineTo(8,2);
    pointPath_big.lineTo(8,-2);

    //设置画笔，画笔默认NOPEN
    painter.setPen(QColor(255,255,255));
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);

    //绘制31个小点
    for(int i=0;i<31;++i){
        QPointF point(0,0);
        painter.save();
        point.setX(radius*qCos(((240+i*2)*M_PI)/180));
        point.setY(radius*qSin(((240+i*2)*M_PI)/180));

        painter.translate(point.x(), point.y());
//        坐标系旋转
        painter.rotate(60+i*2);

        if(i>5 | i<25)
        {
          painter.setBrush(QColor(255,255,255));
        }
        if(i<=5 | i>=25)
        {
          painter.setBrush(QColor(235,70,70));
        }

        if(i%5 == 0)
        {
            painter.drawPath(pointPath_big);//绘画大刻度（其实是个矩形）

            painter.drawText(8, -20, 50, 40,Qt::AlignCenter,QString::number(2*i-30));
        }else
        {
//            painter.drawPath(pointPath_small);//绘画小刻度（其实是个矩形）
        }
        painter.restore();
    }
    painter.setPen(Qt::NoPen);
}



//渐变发光外扇形
void ShipPanel::DrawCircle(QPainter & painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath outRing;
    QPainterPath inRing;
    outRing.moveTo(0,0);
    inRing.moveTo(0,0);
    outRing.arcTo(-radius,-radius, 2*radius,2*radius,60,60);
    inRing.addEllipse(-radius+50,-radius + 50,2*(radius-50),2*(radius-50));
    outRing.closeSubpath();
    //设置渐变色k
    QRadialGradient radialGradient(0,0,radius,0,0);
    radialGradient.setColorAt(1,QColor(0,82,199));
    radialGradient.setColorAt(0.92,Qt::transparent);
    //设置画刷
    painter.setBrush(radialGradient);
    //大圆减小圆
    painter.drawPath(outRing.subtracted(inRing));
    painter.restore();
}

//单位和标题显示
void ShipPanel:: DrawUnit(QPainter & painter, int radius)
{
    painter.save();
    painter.setPen(QColor(255,255,255));
    painter.rotate(-degRotate+30);
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.translate(QPoint(0,0).x(),QPoint(0,0).y() );
    QString num = QString::number(degRotate-30);

    painter.drawText(-this->width()/2, radius*10-radius/7*10, radius/7*80, radius/7*10,Qt::AlignLeft,QString("舵角：%1 °").arg(num));
    painter.restore();
}



//指针
void ShipPanel::  DrawPointer(QPainter &painter, int radius)
{
    //组装点的路径图
    QPainterPath pointPath;
    pointPath.moveTo(10,-radius/4);
    pointPath.lineTo(-10,-radius/4);
    pointPath.lineTo(-1,0);
    pointPath.lineTo(1,0);
    painter.save();

    //计算并选择绘图对象坐标
    QPoint point(0,0);
    point.setX(radius*qCos(((240+degRotate)*M_PI)/180));
    point.setY(radius*qSin(((240+degRotate)*M_PI)/180));
    painter.translate(point.x(), point.y());
    painter.rotate(degRotate - 30);
    painter.setBrush(QColor(102,255,51));
    painter.drawPath(pointPath);
    painter.restore();
}
//最外圆细线
void ShipPanel::DrawCircle_line(QPainter &painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath outRing;
    QPainterPath inRing;
    outRing.moveTo(0,0);
    inRing.moveTo(0,0);
    outRing.arcTo(-radius,-radius, 2*radius,2*radius,60,60);
    inRing.addEllipse(-radius+2,-radius+2,2*(radius-2),2*(radius-2));
    outRing.closeSubpath();

    //设置画刷
    painter.setBrush(QColor(5,228,255));
    //大圆减小圆
    painter.drawPath(outRing.subtracted(inRing));
    painter.restore();
}

void ShipPanel::DrawVScale(QPainter &painter, int d)
{
    QPainterPath pointPath_big;
    pointPath_big.moveTo(-2*d,-1);
    pointPath_big.lineTo(-2*d,1);
    pointPath_big.lineTo(2*d,1);
    pointPath_big.lineTo(2*d,-1);

    QPainterPath pointPath_small;
    pointPath_small.moveTo(-d,-1);
    pointPath_small.lineTo(-d,1);
    pointPath_small.lineTo(d,1);
    pointPath_small.lineTo(d,-1);

    for(int i=-10;i<11;++i)
    {
        painter.save();
        QPoint point(0,0);
        point.setY((d/3.5)*(i+pitch));
        painter.translate(point.x(), point.y());
        painter.setBrush(QColor(255,255,255));

        //设置画笔，画笔默认NOPEN
        painter.setPen(QColor(255,255,255));
        QFont font;
        font.setFamily("Arial");
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);

        if(i%10 == 0)
        {
            painter.drawPath(pointPath_big);//绘画大刻度（其实是个矩形）

            painter.drawText(-4*d, -20, 50, 40,Qt::AlignCenter,QString::number(i));
        }
        else if(i%5 == 0)
        {
            painter.drawPath(pointPath_small);//绘画小刻度（其实是个矩形）

            painter.drawText(-4*d, -20, 50, 40,Qt::AlignCenter,QString::number(i));
        }
        painter.restore();

    }
}

void ShipPanel::DrawBG(QPainter &painter, int l, int r)
{
    int w = this->width()/2;
    int h = this->height();

    QPoint points_top[4] = {
        QPoint(0,0),
        QPoint(0,l+w*qTan((30-degRotate)*M_PI/180)),
        QPoint(2*w, r-w*qTan((30-degRotate)*M_PI/180)),
        QPoint(2*w, 0),
    };
    QPoint points_bottom[4] = {
        QPoint(0,l+w*qTan((30-degRotate)*M_PI/180)),
        QPoint(0,h),
        QPoint(2*w, h),
        QPoint(2*w,r-w*qTan((30-degRotate)*M_PI/180)),
    };

    painter.save();
    //设置线性渐变色k
    QLinearGradient lineGradient(w-h/2*qTan((30-degRotate)*M_PI/180),0,w,h/2);
    lineGradient.setColorAt(0,QColor(0,135,189));
    lineGradient.setColorAt(1,QColor(158,194,208));
    //设置画刷
    painter.setBrush(lineGradient);
    painter.drawPolygon(points_top, 4);
    painter.setBrush(QBrush(QColor(0,105,148)));
    painter.drawPolygon(points_bottom, 4);

    painter.restore();
}

void ShipPanel::DrawBaseLines(QPainter &painter)
{
    painter.save();

    QPoint points[7] = {
        QPoint(-150,0),
        QPoint(-100,0),
        QPoint(150,0),
        QPoint(100,0),
        QPoint(0,0),
        QPoint(-10, 20),
        QPoint(10,20),
    };
    QPen pen(QColor(238,210,2), 3);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawLine(points[0], points[1]);
    painter.drawLine(points[2], points[3]);
    painter.rotate(degRotate-30);
    painter.drawLine(points[4], points[5]);
    painter.drawLine(points[4], points[6]);

    painter.restore();
}



void ShipPanel::degUpdate()
{
    if(!HDirection)
    {
        //left
        degRotate--;
        if(degRotate<0){
            HDirection = 1;
        }
    }
    else
    {
        //right
        degRotate++;
        if(degRotate>60)
        {
            HDirection = 0;
        }
    }

    update();//很重要，通过update方法自动调用paintEvent
}

void ShipPanel::pitchUpdate()
{
    if(VDirection){
        //up
        pitch++;
        if(pitch > 10)
        {
            VDirection=0;
        }
    }
    else
    {
        //down
        pitch--;
        if(pitch<-10)
        {
            VDirection = 1;
        }

    }
    update();
}