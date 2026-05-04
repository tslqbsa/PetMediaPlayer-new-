#include "talkbubble.h"
#include <QRandomGenerator>

TalkBubble::TalkBubble(QWidget *parent):
    QLabel(parent)
{
    setStyleSheet(
        "QLabel {"
        "background-color: white;"
        "border: 2px solid black;"
        "border-radius: 10px;"
        "padding: 6px;"
        "font-size: 16px;"
        "color: black;"
        "}"
        );
    Words = {
        "别戳我",
        "别烦我",
        "嘶",
        "曼波~"
    };
    hide();
    HideTimer = new QTimer(this);
    HideTimer->setSingleShot(true);

    connect(HideTimer, &QTimer::timeout, this, &QLabel::hide);
}
//对话框设置
void TalkBubble::ShowText(const QString& text, int interval)
{
    HideTimer->stop(); // 关键：取消上一次隐藏任务

    setText(text);
    adjustSize();
    show();

    HideTimer->start(interval);
}
//随机对话
void TalkBubble::ShowRandomText(int interval)
{
    int index = QRandomGenerator::global()->bounded(Words.size());

    ShowText(Words[index], interval);
}
