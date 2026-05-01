#include "widget.h"
#include "./ui_widget.h"
#include "petwidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->hide();
}

Widget::~Widget()
{
    delete ui;
}