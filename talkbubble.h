#ifndef TALKBUBBLE_H
#define TALKBUBBLE_H

#include <QLabel>
#include <QTimer>

class TalkBubble : public QLabel
{
    Q_OBJECT

public:
    explicit TalkBubble(QWidget *parent = nullptr);

    void ShowText(const QString &text, int interval = 2000);
};

#endif // TALKBUBBLE_H