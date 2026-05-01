#include "petwidget.h"

#include <QPixmap>
#include <QDebug>
#include <QRandomGenerator>
#include <QStringList>
#include <QTimer>
#include <QApplication>

PetWidget::PetWidget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化窗口
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(300, 300);

    PetLabel = new QLabel(this);

    // 初始化控制面板
    ControlPanel = new Widget();
    ControlPanel->hide();

    // 初始化图片
    SetPetImage(ImageManager.BaseImage());
    CurrentState = PetState::Idle;

    // 初始化对话框
    Bubble = new TalkBubble(this);
    Bubble->move(80, 20);
    Bubble->hide();

    // 初始化托盘
    Tray = new TrayManager(this, this);

    // 初始化定时器
    TimerManager = new PetTimerManager(this);

    connect(TimerManager, &PetTimerManager::IdleTimeout, this, [this]() {
        if (CurrentState != PetState::Idle) {
            return;
        }
        SetPetImage(ImageManager.NextIdleImage());
    });

    connect(TimerManager, &PetTimerManager::AngryTimeout, this, [this]() {
        ChangeState(PetState::Idle);
    });

    connect(TimerManager, &PetTimerManager::TalkTimeout, this, [this]() {
        // 只有待机状态才随机说话
        if (CurrentState == PetState::Idle) {
            Bubble->ShowRandomText(2000);
        }

        // 随机下一次说话时间
        TimerManager->StartRandomTalk();
    });

    connect(TimerManager, &PetTimerManager::SleepTimeout, this, [this]() {
        ChangeState(PetState::Sleep);
    });

    TimerManager->StartIdleAnimation();
    TimerManager->StartRandomTalk();
    TimerManager->ResetSleepTimer();
}

void PetWidget::OpenControlPanel()
{
    ControlPanel->show();
    ControlPanel->raise();
    ControlPanel->activateWindow();
}

void PetWidget::ChangeState(PetState state)
{
    CurrentState = state;

    if (state == PetState::Idle) {
        SetPetImage(ImageManager.BaseImage());
        TimerManager->StartIdleAnimation();
    }
    else if (state == PetState::Angry) {
        TimerManager->StopIdleAnimation();
        SetPetImage(ImageManager.AngryImage());
        TimerManager->StartAngryTimer();
    }
    else if (state == PetState::Drag) {
        TimerManager->StopIdleAnimation();
        SetPetImage(ImageManager.DragImage());
    }
    else if (state == PetState::Sleep) {
        TimerManager->StopIdleAnimation();
        SetPetImage(ImageManager.SleepImage());
    }
}

void PetWidget::SetPetImage(const QString &path)
{
    QPixmap pix(path);

    if (pix.isNull()) {
        qDebug() << "图片加载失败:" << path;
        return;
    }

    pix = pix.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    PetLabel->setPixmap(pix);
    PetLabel->resize(pix.size());
    PetLabel->move((width() - PetLabel->width()) / 2,
                   (height() - PetLabel->height()) / 2);
}

void PetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        // 有操作就重置睡眠计时
        TimerManager->ResetSleepTimer();

        DragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();

        // 如果正在睡觉，点击只负责唤醒
        if (CurrentState == PetState::Sleep) {
            ChangeState(PetState::Idle);
            Bubble->ShowText("醒啦...", 1000);
            return;
        }

        // 随机台词
        Bubble->ShowRandomText(1000);

        // 点击时进入生气状态
        ChangeState(PetState::Angry);
    }
}

void PetWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        // 拖动时进入拖动状态
        if (CurrentState != PetState::Drag) {
            ChangeState(PetState::Drag);
        }

        move(event->globalPosition().toPoint() - DragPosition);
    }
}

void PetWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (CurrentState == PetState::Drag) {
        ChangeState(PetState::Idle);
    }
}

void PetWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *ShowAction = menu.addAction("显示桌宠");
    QAction *HideAction = menu.addAction("隐藏桌宠");

    menu.addSeparator();

    QAction *SleepAction = menu.addAction("睡觉");
    QAction *WakeAction = menu.addAction("唤醒");

    menu.addSeparator();

    QAction *SettingAction = menu.addAction("设置");
    QAction *QuitAction = menu.addAction("退出");

    QAction *selected = menu.exec(event->globalPos());

    if (selected == ShowAction) {
        this->show();
    }
    else if (selected == HideAction) {
        this->hide();
    }
    else if (selected == SleepAction) {
        ChangeState(PetState::Sleep);
    }
    else if (selected == WakeAction) {
        ChangeState(PetState::Idle);
        TimerManager->ResetSleepTimer();
    }
    else if (selected == QuitAction) {
        QApplication::quit();
    }
    else if (selected == SettingAction) {
        OpenControlPanel();
    }
}
void PetWidget::Sleep()
{
    ChangeState(PetState::Sleep);
}

void PetWidget::Wake()
{
    ChangeState(PetState::Idle);
    TimerManager->ResetSleepTimer();
}