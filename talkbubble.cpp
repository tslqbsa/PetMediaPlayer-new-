#include "talkbubble.h"
#include <QRandomGenerator>
#include <QFontMetrics>
#include <QRect>
#include <QtGlobal>

TalkBubble::TalkBubble(QWidget *parent):
    QLabel(parent)
{
    UpdateBubbleStyle();

    Words = {
        "别戳我",
        "别烦我",
        "嘶",
        "曼波~"
    };

    OpacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(OpacityEffect);
    OpacityEffect->setOpacity(0.0);

    FadeInAnimation = new QPropertyAnimation(OpacityEffect, "opacity", this);
    FadeInAnimation->setDuration(180);
    FadeInAnimation->setStartValue(0.0);
    FadeInAnimation->setEndValue(1.0);

    FadeOutAnimation = new QPropertyAnimation(OpacityEffect, "opacity", this);
    FadeOutAnimation->setDuration(260);
    FadeOutAnimation->setStartValue(1.0);
    FadeOutAnimation->setEndValue(0.0);

    connect(FadeOutAnimation, &QPropertyAnimation::finished, this, &QLabel::hide);

    hide();

    HideTimer = new QTimer(this);
    HideTimer->setSingleShot(true);

    connect(HideTimer, &QTimer::timeout, this, [this]() {
        FadeInAnimation->stop();
        FadeOutAnimation->stop();
        FadeOutAnimation->start();
    });
}

// 对话框设置
void TalkBubble::ShowText(const QString& text, int interval)
{
    HideTimer->stop();

    // 如果显示的是同一句话，就只刷新隐藏时间，避免闪烁
    if (text == CurrentText && isVisible()) {
        FadeOutAnimation->stop();
        OpacityEffect->setOpacity(1.0);
        HideTimer->start(interval);
        return;
    }

    CurrentText = text;

    setText(text);
    setWordWrap(true);  // 自动换行

    // 根据文字内容动态计算气泡大小
    ResizeToFitText();

    FadeInAnimation->stop();
    FadeOutAnimation->stop();

    if (!isVisible()) {
        OpacityEffect->setOpacity(0.0);
        show();
        FadeInAnimation->start();
    }
    else {
        OpacityEffect->setOpacity(1.0);
    }

    raise();

    HideTimer->start(interval);
}

void TalkBubble::SetBubbleScale(int scale)
{
    BubbleScale = qBound(50, scale, 150);

    UpdateBubbleStyle();

    if (!CurrentText.isEmpty()) {
        ResizeToFitText();
    }
}

void TalkBubble::UpdateBubbleStyle()
{
    int BorderWidth = 2 * BubbleScale / 100;
    int BorderRadius = 10 * BubbleScale / 100;
    int Padding = 6 * BubbleScale / 100;
    int FontSize = 16 * BubbleScale / 100;

    setStyleSheet(QString(
                      "QLabel {"
                      "background-color: white;"
                      "border: %1px solid black;"
                      "border-radius: %2px;"
                      "padding: %3px;"
                      "font-size: %4px;"
                      "color: black;"
                      "}"
                      ).arg(BorderWidth)
                      .arg(BorderRadius)
                      .arg(Padding)
                      .arg(FontSize));
}

void TalkBubble::ResizeToFitText()
{
    QFontMetrics FontMetrics(font());

    int MinWidth = 90 * BubbleScale / 100;
    int MaxWidth = 360 * BubbleScale / 100;
    int PaddingX = 28 * BubbleScale / 100;
    int PaddingY = 18 * BubbleScale / 100;

    int TextWidth = FontMetrics.horizontalAdvance(CurrentText) + PaddingX;
    int BubbleWidth = qBound(MinWidth, TextWidth, MaxWidth);

    QRect TextRect = FontMetrics.boundingRect(
        QRect(0, 0, BubbleWidth - PaddingX, 1000),
        Qt::TextWordWrap,
        CurrentText
        );

    int BubbleHeight = TextRect.height() + PaddingY;

    setFixedSize(BubbleWidth, BubbleHeight);
}

// 随机对话
void TalkBubble::ShowRandomText(int interval)
{
    int index = QRandomGenerator::global()->bounded(Words.size());

    ShowText(Words[index], interval);
}