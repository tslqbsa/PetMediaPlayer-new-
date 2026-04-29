#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class PetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PetWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;//鼠标按压
    void mouseMoveEvent(QMouseEvent *event) override;//鼠标移动

private:
    QLabel *PetLabel;//桌宠
    QPoint DragPosition;//落下位置
};

#endif // PETWIDGET_H
