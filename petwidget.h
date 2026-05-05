#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

#include "widget.h"
#include "talkbubble.h"
#include "petimagemanager.h"
#include "pettimermanager.h"
#include "traymanager.h"

enum class PetState
{
    Idle,        // 普通待机
    Sleep,       // 睡觉
    Angry,       // 生气
    Drag,        // 拖动
    ListenIdle,  // 听歌待机
    ListenAngry,  // 听歌生气
    ListenDrag // 听歌拖动
};

class PetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PetWidget(QWidget *parent = nullptr);
    void Sleep();
    void Wake();//暴露给tray
    void OpenControlPanel();
    void StartListen(); // 进入听歌状态
    void StopListen();  // 退出听歌状态
    void ShowLyric(const QString &text, int duration); // 显示歌词气泡

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void ChangeState(PetState state);
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    //主菜单
    Widget *ControlPanel;
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

    // 是否处于听歌模式
    bool IsListening;
};

#endif // PETWIDGET_H