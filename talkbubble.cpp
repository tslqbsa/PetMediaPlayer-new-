#include "talkbubble.h"

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

    hide();
}
//对话框设置
void TalkBubble::ShowText(const QString& text,int interval){
    setText(text);
    adjustSize();
    show();
    QTimer::singleShot(interval, this, &QLabel::hide);
}
