#include "talkbubble.h"
#include <QRandomGenerator>
#include <QFontMetrics>
#include <QRect>

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

// 对话框设置
void TalkBubble::ShowText(const QString& text, int interval)
{
    HideTimer->stop();

    // 如果显示的是同一句话，就只刷新隐藏时间，避免闪烁
    if (text == CurrentText && isVisible()) {
        HideTimer->start(interval);
        return;
    }

    CurrentText = text;

    setText(text);
    setWordWrap(true);  // 自动换行

    // 根据文字内容动态计算气泡大小
    QFontMetrics FontMetrics(font());

    int MinWidth = 90;
    int MaxWidth = 360;
    int PaddingX = 28;
    int PaddingY = 18;

    // 先按单行文字宽度估算气泡宽度
    int TextWidth = FontMetrics.horizontalAdvance(text) + PaddingX;

    // 限制气泡宽度在 MinWidth 和 MaxWidth 之间
    int BubbleWidth = qBound(MinWidth, TextWidth, MaxWidth);

    // 根据最终宽度计算自动换行后的文字高度
    QRect TextRect = FontMetrics.boundingRect(
        QRect(0, 0, BubbleWidth - PaddingX, 1000),
        Qt::TextWordWrap,
        text
        );

    int BubbleHeight = TextRect.height() + PaddingY;

    // 设置最终气泡大小
    setFixedSize(BubbleWidth, BubbleHeight);

    if (!isVisible()) {
        show();
    }

    raise();

    HideTimer->start(interval);
}

// 随机对话
void TalkBubble::ShowRandomText(int interval)
{
    int index = QRandomGenerator::global()->bounded(Words.size());

    ShowText(Words[index], interval);
}