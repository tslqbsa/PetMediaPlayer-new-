#include "widget.h"
#include "./ui_widget.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(300, 300);

    QLabel *label = new QLabel(this);

    QPixmap pix(":/resources/images/pet.png");

    if (pix.isNull()) {
        qDebug() << "图片加载失败";
        return;
    }

    pix = pix.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    label->setPixmap(pix);
    label->resize(pix.size());
    label->move((width() - label->width()) / 2,
                (height() - label->height()) / 2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        DragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();

    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - DragPosition);
    }
}
