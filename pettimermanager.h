#ifndef PETTIMERMANAGER_H
#define PETTIMERMANAGER_H

#include <QObject>
#include <QTimer>

class PetTimerManager : public QObject
{
    Q_OBJECT

public:
    explicit PetTimerManager(QObject *parent = nullptr);

    void StartIdleAnimation(int interval = 800);
    void StopIdleAnimation();

    void StartRandomTalk(int minTime = 10000, int maxTime = 20000);
    void ResetSleepTimer(int interval = 30000);

    void StartAngryTimer(int interval = 1000);

signals:
    void IdleTimeout();
    void TalkTimeout();
    void SleepTimeout();
    void AngryTimeout();

private:
    QTimer *IdleTimer;
    QTimer *TalkTimer;
    QTimer *SleepTimer;
    QTimer *AngryTimer;
};

#endif // PETTIMERMANAGER_H