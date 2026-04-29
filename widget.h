#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
protected:
    void mousePressEvent(QMouseEvent *event) override; // 鼠标按下
    void mouseMoveEvent(QMouseEvent *event) override;  // 鼠标移动
private:
    Ui::Widget *ui;
    QPoint DragPosition;//落下位置
    QLabel *TalkLabel;//对话label
};
#endif // WIDGET_H



