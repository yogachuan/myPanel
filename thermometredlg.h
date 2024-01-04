#ifndef THERMOMETREDLG_H
#define THERMOMETREDLG_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#define YELLOW QColor(Qt::yellow)
#define WHITE QColor(Qt::white)
#define RED QColor(Qt::red)
#define GREEN QColor(Qt::green)
#define BLACK QColor(Qt::black)


class ThermometreDlg : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ getValue WRITE setValue)  //声明属性，有动画时需要
public:
    explicit ThermometreDlg(QWidget *parent = nullptr);
    qreal getValue();

    //有动画时，会通过Q_PROPERTY(qreal value READ getValue WRITE setValue)，自动调用WRITE
    //动画的原理就是注册属性时注册的WRITE函数不断被调用从而改变属性值。
    void setValue(qreal value);

    void valueUpdate(qreal value);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void startAnimation();
private:
    void updateRect();
    void drawRect(QPainter &painter);
    void drawBottom(QPainter &painter);
    void drawScale(QPainter &painter);

    QFont setFont(QString fontName, int size, bool bold=false);

private:
    qreal m_value;
    qreal curValue;
    int m_width;
    QRectF m_rect;
    int maxValue, minValue;
    qreal m_radius;
    QPropertyAnimation *m_valueAnimation;
    int flag = 1; //0下降，1上升


signals:

};

#endif // THERMOMETREDLG_H
