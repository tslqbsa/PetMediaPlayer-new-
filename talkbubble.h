#ifndef TALKBUBBLE_H
#define TALKBUBBLE_H

#include <QLabel>
#include <QTimer>
#include <QStringList>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class TalkBubble : public QLabel
{
    Q_OBJECT

public:
    explicit TalkBubble(QWidget *parent = nullptr);

    void ShowText(const QString &text, int interval = 2000);
    void ShowRandomText(int interval = 1000);
    QString CurrentText; // 当前显示内容
    void SetBubbleScale(int scale);

private:
    QStringList Words;
    QTimer *HideTimer; // 控制气泡隐藏
    QGraphicsOpacityEffect *OpacityEffect = nullptr;
    QPropertyAnimation *FadeInAnimation = nullptr;
    QPropertyAnimation *FadeOutAnimation = nullptr;

    int BubbleScale = 100;

    void UpdateBubbleStyle();
    void ResizeToFitText();
};
#endif // TALKBUBBLE_H