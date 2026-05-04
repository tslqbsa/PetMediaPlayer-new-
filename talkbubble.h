#ifndef TALKBUBBLE_H
#define TALKBUBBLE_H

#include <QLabel>
#include <QTimer>
#include <QStringList>

class TalkBubble : public QLabel
{
    Q_OBJECT

public:
    explicit TalkBubble(QWidget *parent = nullptr);

    void ShowText(const QString &text, int interval = 2000);
    void ShowRandomText(int interval = 1000);

private:
    QStringList Words;
    QTimer *HideTimer; // 控制气泡隐藏
};
#endif // TALKBUBBLE_H