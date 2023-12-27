#ifndef THERMOMETREDLG_H
#define THERMOMETREDLG_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QPainter>
#include <QTimer>
#include <QDebug>

class ThermometreDlg : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ getValue WRITE setValue)  //声明属性
public:
    explicit ThermometreDlg(QWidget *parent = nullptr);
    qreal getValue();
    void setValue(qreal value);
    void changeValue(qreal value);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void startAnimation();

private:
    qreal m_value;
    qreal curValue;
    int m_width;
    QRectF m_rect;
    int maxValue, minValue;
    qreal m_radius;
    QPropertyAnimation *m_valueAnimation;
    void updateRect();


signals:

};

#endif // THERMOMETREDLG_H
