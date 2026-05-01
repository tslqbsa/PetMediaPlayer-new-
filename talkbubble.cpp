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
}
//对话框设置
void TalkBubble::ShowText(const QString& text,int interval){
    setText(text);
    adjustSize();
    show();
    QTimer::singleShot(interval, this, &QLabel::hide);
}
//随机对话
void TalkBubble::ShowRandomText(int interval)
{
    int index = QRandomGenerator::global()->bounded(Words.size());

    ShowText(Words[index], interval);
}
