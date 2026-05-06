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
    this->resize(420, 360);

    PetLabel = new QLabel(this);

    // 初始化对话框
    Bubble = new TalkBubble(this);
    Bubble->SetBubbleScale(PetScale);
    UpdateBubblePosition();
    Bubble->hide();

    // 初始化控制面板
    ControlPanel = new Widget();
    ControlPanel->SetPetWidget(this);
    ControlPanel->hide();

    // 初始化图片
    SetPetImage(ImageManager.BaseImage());
    CurrentState = PetState::Idle;
    IsListening = false;

    // 初始化托盘
    Tray = new TrayManager(this, this);

    // 初始化定时器
    TimerManager = new PetTimerManager(this);

    connect(TimerManager, &PetTimerManager::IdleTimeout, this, [this]() {
        if (CurrentState == PetState::Idle) {
            SetPetImage(ImageManager.NextIdleImage());
        }
        else if (CurrentState == PetState::ListenIdle) {
            SetPetImage(ImageManager.NextListenIdleImage());
        }
    });

    connect(TimerManager, &PetTimerManager::AngryTimeout, this, [this]() {
        if (CurrentState == PetState::Drag ||
            CurrentState == PetState::ListenDrag) {
            return;
        }

        if (IsListening) {
            ChangeState(PetState::ListenIdle);
        }
        else {
            ChangeState(PetState::Idle);
        }
    });

    connect(TimerManager, &PetTimerManager::TalkTimeout, this, [this]() {
        // 听歌时不随机说话，避免打断歌词
        if (!IsListening && CurrentState == PetState::Idle) {
            Bubble->ShowRandomText(2000);
            UpdateBubblePosition();
        }

        // 随机下一次说话时间
        TimerManager->StartRandomTalk();
    });

    connect(TimerManager, &PetTimerManager::SleepTimeout, this, [this]() {
        if (IsListening) {
            TimerManager->ResetSleepTimer();
            return;
        }

        ChangeState(PetState::Sleep);
    });

    TimerManager->StartIdleAnimation();
    TimerManager->StartRandomTalk();
    TimerManager->ResetSleepTimer();

    //初始化动画
    ListenMoveTimer = new QTimer(this);
    ListenMoveDirection = false;

    connect(ListenMoveTimer, &QTimer::timeout, this, [this]() {
        int offset = ListenMoveDirection ? 3 : -3;
        move(ListenBasePosition.x() + offset, ListenBasePosition.y());
        ListenMoveDirection = !ListenMoveDirection;
    });
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
    else if (state == PetState::ListenIdle) {
        SetPetImage(ImageManager.NextListenIdleImage());
        TimerManager->StartIdleAnimation();
    }
    else if (state == PetState::ListenAngry) {
        TimerManager->StopIdleAnimation();
        SetPetImage(ImageManager.ListenAngryImage());
        TimerManager->StartAngryTimer();
    }
    else if (state == PetState::ListenDrag) {
        TimerManager->StopIdleAnimation();
        SetPetImage(ImageManager.ListenDragImage());
    }
}

void PetWidget::SetPetImage(const QString &path)
{
    CurrentImagePath = path;

    QPixmap pix(path);

    if (pix.isNull()) {
        qDebug() << "图片加载失败:" << path;
        return;
    }

    int TargetSize = 300 * PetScale / 100;

    pix = pix.scaled(TargetSize, TargetSize,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);

    PetLabel->setPixmap(pix);
    PetLabel->resize(pix.size());
    int MarginTop = 120 * PetScale / 100;
    int MarginSide = 100 * PetScale / 100;
    int MarginBottom = 20 * PetScale / 100;

    resize(PetLabel->width() + MarginSide * 2,
           PetLabel->height() + MarginTop + MarginBottom);

    PetLabel->move((width() - PetLabel->width()) / 2,
                   MarginTop);

    UpdateBubblePosition();
}

void PetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        // 有操作就重置睡眠计时
        TimerManager->ResetSleepTimer();

        // 记录鼠标按下时的位置
        PressGlobalPosition = event->globalPosition().toPoint();

        // 记录鼠标相对窗口左上角的位置，用于拖动
        DragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();

        // 一开始不认为是拖动
        IsDragging = false;

        // 如果正在睡觉，点击只负责唤醒
        if (CurrentState == PetState::Sleep) {
            ChangeState(PetState::Idle);
            Bubble->ShowText("醒啦...", 1000);
            return;
        }
    }
}

void PetWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {

        // 计算鼠标从按下到现在移动了多远
        int MoveDistance =
            (event->globalPosition().toPoint() - PressGlobalPosition).manhattanLength();

        // 移动距离超过 6，才认为是真的拖动
        if (MoveDistance > 6) {
            IsDragging = true;
        }

        // 如果还没达到拖动阈值，不移动桌宠
        if (!IsDragging) {
            return;
        }

        // 开始拖动时关闭摇摆动画
        ListenMoveTimer->stop();

        if (CurrentState != PetState::Drag &&
            CurrentState != PetState::ListenDrag) {

            if (IsListening) {
                ChangeState(PetState::ListenDrag);
            }
            else {
                ChangeState(PetState::Drag);
            }
        }

        move(event->globalPosition().toPoint() - DragPosition);
    }
}

void PetWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    // 如果刚才发生了拖动，松手后恢复状态
    if (IsDragging) {
        if (IsListening) {
            ListenBasePosition = pos(); // 更新听歌摇摆基准位置
            ChangeState(PetState::ListenIdle);
            ListenMoveTimer->start(300);
        }
        else {
            ChangeState(PetState::Idle);
        }

        IsDragging = false;
        return;
    }

    // 如果没有拖动，说明这是一次普通点击

    // 随机台词
    if (!IsListening) {
        Bubble->ShowRandomText(1000);
        UpdateBubblePosition();
    }

    // 点击时进入生气状态
    if (IsListening) {
        ChangeState(PetState::ListenAngry);
    }
    else {
        ChangeState(PetState::Angry);
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
    else if (selected == QuitAction) {
        QApplication::quit();
    }
    else if (selected == SettingAction) {
        OpenControlPanel();
    }
    else if (selected == SleepAction) {
        if (ControlPanel != nullptr) {
            ControlPanel->PauseMusic();
        }

        IsListening = false;
        ListenMoveTimer->stop();
        Bubble->hide();

        ChangeState(PetState::Sleep);
    }
    else if (selected == WakeAction) {
        IsListening = false;
        ListenMoveTimer->stop();

        ChangeState(PetState::Idle);
        TimerManager->ResetSleepTimer();
    }

}
void PetWidget::Sleep()
{
    IsListening = false;
    ChangeState(PetState::Sleep);
}

void PetWidget::Wake()
{
    IsListening = false;
    ChangeState(PetState::Idle);
    TimerManager->ResetSleepTimer();
}

void PetWidget::StartListen()
{
    IsListening = true;
    ListenBasePosition = pos();
    ListenMoveTimer->start(300);
    ChangeState(PetState::ListenIdle);
}

void PetWidget::StopListen()
{
    IsListening = false;
    ListenMoveTimer->stop();
    move(ListenBasePosition);
    ChangeState(PetState::Idle);
}
void PetWidget::ShowLyric(const QString &text, int duration)
{
    Bubble->ShowText(text, duration);
    UpdateBubblePosition();
}

void PetWidget::SetLyricBubbleVisible(bool visible)
{
    if (Bubble == nullptr) {
        return;
    }

    if (!visible) {
        Bubble->hide();
    }

    Bubble->setVisible(visible);
}

void PetWidget::SetPetScale(int scale)
{
    PetScale = qBound(50, scale, 150);

    if (Bubble != nullptr) {
        Bubble->SetBubbleScale(PetScale);
    }

    if (!CurrentImagePath.isEmpty()) {
        SetPetImage(CurrentImagePath);
    }

    UpdateBubblePosition();
}
void PetWidget::UpdateBubblePosition()
{
    if (Bubble == nullptr || PetLabel == nullptr) {
        return;
    }

    int Gap = 0;

    int BubbleX = PetLabel->x() + PetLabel->width() / 2 - Bubble->width() / 2;

    int HeadOffset = 70 * PetScale / 100;
    int BubbleY = PetLabel->y() + HeadOffset - Bubble->height() - Gap;

    int MaxBubbleX = width() - Bubble->width();

    if (MaxBubbleX < 0) {
        BubbleX = 0;
    }
    else {
        BubbleX = qBound(0, BubbleX, MaxBubbleX);
    }

    if (BubbleY < 0) {
        BubbleY = 0;
    }

    Bubble->move(BubbleX, BubbleY);
}
void PetWidget::SetAlwaysOnTop(bool enabled)
{
    Qt::WindowFlags Flags = windowFlags();

    if (enabled) {
        Flags |= Qt::WindowStaysOnTopHint;
    }
    else {
        Flags &= ~Qt::WindowStaysOnTopHint;
    }

    setWindowFlags(Flags);
    show();
}