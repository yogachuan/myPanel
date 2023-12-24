#include "speedpanel.h"



SpeedPanel::SpeedPanel(QWidget *parent) : QWidget(parent)
{
    //设置背景
    m_bg = QPixmap(":/res/background.png");
    QPalette pal = this->palette();
//    pal.setBrush(QPalette::Window, QBrush(m_bg));
    pal.setBrush(QPalette::Background, QBrush(m_bg));//win系统
    setPalette(pal);
//    setFixedSize(1280,800);

    myTimer = new QTimer(this);
    myTimer->start(1);
    connect(myTimer, &QTimer::timeout, this, [=]{
        degChanged();
    });

}


void SpeedPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), m_bg, QRect());
    int width=this->width();
    int height=this->height();//移动仪表盘的高度
    int radius=((width>height)?height:width)/2.0;//仪表盘的中心位置                                                                                                                                                                                                 ;
    //移动画笔到中下方
    painter.translate(width/2,height/2);
    //启用反锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    DrawSmallScale(painter,radius*0.75);//刻度线
    DrawCircle(painter,radius*0.9);//渐变发光外扇形
    DrawCircle_arc(painter,radius*0.9);//动态扇形环
    DrawPointer(painter,radius*0.6);//指针
    DrawCircle_line(painter,radius*0.9); //最外细圆线
    DrawCircle_bom_big(painter,radius/7*4);//中间大圆
    DrawCircle_bom_shine(painter,radius*0.35);//渐变发光内圈
    DrawCircle_bom_small(painter,radius/7*3);//中间小圆及时速
    DrawUnit(painter, -radius*0.1);//单位和标题显示

}


//绘制刻度
void SpeedPanel::DrawSmallScale(QPainter& painter,int radius)
{
    //组装点的路径图
    QPainterPath pointPath_small;
    pointPath_small.moveTo(-2,-2);
    pointPath_small.lineTo(2,-2);
    pointPath_small.lineTo(2,8);
    pointPath_small.lineTo(-2,8);


    QPainterPath pointPath_big;
    pointPath_big.moveTo(-2,-2);
    pointPath_big.lineTo(2,-2);
    pointPath_big.lineTo(2,20);
    pointPath_big.lineTo(-2,20);


    painter.setPen(QColor(255,255,255));
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    //绘制31个小点
    for(int i=0;i<31;++i){
        QPointF point(0,0);
        painter.save();
        point.setX(radius*qCos(((210-i*8)*M_PI)/180));
        point.setY(radius*qSin(((210-i*8)*M_PI)/180));

        //坐标系平移
        painter.translate(point.x(),-point.y());
//        坐标系旋转
        painter.rotate(-120+i*8);

        i<20? painter.setBrush(QColor(255,255,255)) : painter.setBrush(QColor(235,70,70));


        if(i%5 == 0)
        {
            painter.drawPath(pointPath_big);//绘画大刻度（其实是个矩形）

            painter.drawText(-25, 20, 50, 20,Qt::AlignCenter,QString::number(i));
        }else
        {
            painter.drawPath(pointPath_small);//绘画小刻度（其实是个矩形）
        }
        painter.restore();
    }

    painter.setPen(Qt::NoPen);
}


//渐变发光外扇形
void SpeedPanel::DrawCircle(QPainter & painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath outRing;
    QPainterPath inRing;
    outRing.moveTo(0,0);
    inRing.moveTo(0,0);
    outRing.arcTo(-radius,-radius, 2*radius,2*radius,-30,240);
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
void SpeedPanel:: DrawUnit(QPainter & painter, int radius)
{
    painter.save();
    painter.setPen(QColor(255,255,255));
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(radius/7*11, -radius+10, -radius/7*22, -radius/7*8,Qt::AlignCenter,QString("Kn/H"));
    painter.drawText(radius/7*17, -radius/7*19, -radius/7*34, -radius/7*8,Qt::AlignCenter,QString("当前航速"));
    painter.restore();
}


//动态扇形环
void SpeedPanel::DrawCircle_arc(QPainter &painter, int radius)
{
    QRect rect(-radius,-radius,2*radius,2*radius);
    QConicalGradient Conical(0,0,-70);

    Conical.setColorAt(0.1,QColor(255,88,127,200));//红色
    Conical.setColorAt(0.5,QColor(53,179,251,150));//蓝色
    painter.setBrush(Conical);
    painter.drawPie(rect,210*16,-(degRotate)*16);//注意其单位是1/16角度，而不是弧度。
}

//指针
void SpeedPanel::DrawPointer(QPainter &painter, int radius)
{
    //组装点的路径图
    QPainterPath pointPath;
    pointPath.moveTo(10,0);
    pointPath.lineTo(1,-radius);
    pointPath.lineTo(-1,-radius);
    pointPath.lineTo(-10,0);
    pointPath.arcTo(-10,0,20,20,180,180);
    QPainterPath inRing;
    inRing.addEllipse(-5,-5,10,10);
    painter.save();

    //计算并选择绘图对象坐标
    painter.rotate(degRotate - 120);
    painter.setBrush(QColor(255,255,255));
    painter.drawPath(pointPath.subtracted(inRing));
    painter.restore();
}
//最外圆细线
void SpeedPanel::DrawCircle_line(QPainter &painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath outRing;
    QPainterPath inRing;
    outRing.moveTo(0,0);
    inRing.moveTo(0,0);
    outRing.arcTo(-radius,-radius, 2*radius,2*radius,-30,240);
    inRing.addEllipse(-radius+2,-radius+2,2*(radius-2),2*(radius-2));
    outRing.closeSubpath();

    //设置画刷
    painter.setBrush(QColor(5,228,255));
    //大圆减小圆
    painter.drawPath(outRing.subtracted(inRing));
    painter.restore();
}

//中间大圆
void SpeedPanel::DrawCircle_bom_big(QPainter &painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath inRing;
    inRing.moveTo(0,0);
    inRing.addEllipse(-radius+50,-radius + 50,2*(radius-50),2*(radius-50));
    //设置画刷
    painter.setBrush(QColor(10,20,30));
    painter.drawPath(inRing);
    painter.restore();
}

//渐变发光内圈
void SpeedPanel::DrawCircle_bom_shine(QPainter &painter, int radius)
{
    painter.save();
    QRadialGradient radialGradient(0,0,radius,0,0);
//    radialGradient.setColorAt(0.5,QColor(8,77,197));
    radialGradient.setColorAt(0.5,QColor(10,68,185,150));
    radialGradient.setColorAt(1.0,Qt::transparent);
    painter.setBrush(QBrush(radialGradient));
    painter.drawRect(-radius,-radius,2*(radius),2*(radius));
    painter.restore();
}

//中间小圆
void SpeedPanel::DrawCircle_bom_small(QPainter &painter, int radius)
{
    //保存绘图对象
    painter.save();
    //计算大小圆路径
    QPainterPath inRing;
    inRing.moveTo(0,0);
    inRing.addEllipse(-radius+50,-radius + 50,2*(radius-50),2*(radius-50));
    //设置画刷
    painter.setBrush(QColor(10,20,30));
    painter.drawPath(inRing);

    //时速
    painter.setPen(QColor(255,255,255));
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(30);
    painter.setFont(font);
    painter.drawText(-radius+50, -radius+50, 2*(radius-50),2*(radius-50),Qt::AlignCenter,QString::number(degRotate/8));
    painter.setPen(Qt::NoPen);
    painter.restore();
}





void SpeedPanel::degChanged()
{
    if(degRotate==240){
        myTimer->stop();
        return;
    }

    degRotate++;
    update();//很重要，通过update方法自动调用paintEvent
}
