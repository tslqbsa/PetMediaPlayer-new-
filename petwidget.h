#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>

#include "talkbubble.h"
#include "petimagemanager.h"
#include "pettimermanager.h"
#include "traymanager.h"

enum class PetState
{
    Idle,     // 待机
    Angry,    // 生气
    Listen,   // 听歌
    Sleep,    // 睡觉
    Drag      // 被拖动
};

class PetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PetWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void ChangeState(PetState state);

private:
    // 图片显示
    QLabel *PetLabel;

    // 对话框
    TalkBubble *Bubble;

    // 图片管理器
    PetImageManager ImageManager;

    // 定时器
    PetTimerManager *TimerManager;

    // 拖动位置
    QPoint DragPosition;

    // 设置图片
    void SetPetImage(const QString &path);

    //当前状态
    PetState CurrentState;

    //托盘
    TrayManager *Tray;
};

#endif // PETWIDGET_H