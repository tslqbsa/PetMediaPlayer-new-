#include "petwidget.h"
#include <QPixmap>
#include <QDebug>
#include <QRandomGenerator>
#include <QStringList>

PetWidget::PetWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(300, 300);

    PetLabel = new QLabel(this);
    // 加载图片
    QPixmap pix(":/resources/images/pet.png");
    if (pix.isNull()) {
        qDebug() << "图片加载失败";
        return;
    }

    pix = pix.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    PetLabel->setPixmap(pix);
    PetLabel->resize(pix.size());
    PetLabel->move((width() - PetLabel->width()) / 2,
                   (height() - PetLabel->height()) / 2);

    //设置对话框
    Bubble = new TalkBubble(this);
    Bubble->move(80, 20);
    Bubble->hide();
}

void PetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        DragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        //随机台词
        QStringList words = {
            "别戳我",
            "别烦我",
            "嘶",
            "曼波~"
        };
        int index = QRandomGenerator::global()->bounded(words.size());
        Bubble->ShowText(words[index],1000);
    }
}

void PetWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - DragPosition);
    }
}