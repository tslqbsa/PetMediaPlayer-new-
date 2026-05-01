#include "petwidget.h"

#include <QPixmap>
#include <QDebug>
#include <QRandomGenerator>
#include <QStringList>
#include <QTimer>

PetWidget::PetWidget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化窗口
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(300, 300);

    PetLabel = new QLabel(this);

    // 初始化图片
    SetPetImage(ImageManager.BaseImage());
    CurrentState = PetState::Idle;

    // 初始化待机动画定时器
    IdleTimer = new QTimer(this);
    connect(IdleTimer, &QTimer::timeout, this, [this]() {
        if (CurrentState != PetState::Idle) {
            return;
        }
        SetPetImage(ImageManager.NextIdleImage());
    });
    IdleTimer->start(800);

    AngryTimer = new QTimer(this);
    AngryTimer->setSingleShot(true);

    connect(AngryTimer, &QTimer::timeout, this, [this]() {
        ChangeState(PetState::Idle);
    });

    // 初始化对话框
    Bubble = new TalkBubble(this);
    Bubble->move(80, 20);
    Bubble->hide();
}

void PetWidget::ChangeState(PetState state)
{
    CurrentState = state;

    if (state == PetState::Idle) {
        SetPetImage(ImageManager.BaseImage());
        IdleTimer->start(800);
    }
    else if (state == PetState::Angry) {
        IdleTimer->stop();
        SetPetImage(ImageManager.AngryImage());
        AngryTimer->start(1000);
    }
    else if(state==PetState::Drag){
        IdleTimer->stop();
        SetPetImage(ImageManager.DragImage());
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
        DragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();

        // 随机台词
        Bubble->ShowRandomText(1000);

        // 点击时先显示生气状态
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