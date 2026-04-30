#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>

#include "talkbubble.h"
#include "petimagemanager.h"

class PetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PetWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // 图片显示
    QLabel *PetLabel;

    // 对话框
    TalkBubble *Bubble;

    // 图片管理器
    PetImageManager ImageManager;

    // 待机动画定时器
    QTimer *IdleTimer;

    // 拖动位置
    QPoint DragPosition;

    // 设置图片
    void SetPetImage(const QString &path);
};

#endif // PETWIDGET_H