
#include "pettimermanager.h"

#include <QRandomGenerator>

PetTimerManager::PetTimerManager(QObject *parent)
    : QObject{parent}
{
    IdleTimer = new QTimer(this);
    TalkTimer = new QTimer(this);
    SleepTimer = new QTimer(this);
    AngryTimer = new QTimer(this);

    SleepTimer->setSingleShot(true);
    AngryTimer->setSingleShot(true);

    connect(IdleTimer, &QTimer::timeout, this, &PetTimerManager::IdleTimeout);
    connect(TalkTimer, &QTimer::timeout, this, &PetTimerManager::TalkTimeout);
    connect(SleepTimer, &QTimer::timeout, this, &PetTimerManager::SleepTimeout);
    connect(AngryTimer, &QTimer::timeout, this, &PetTimerManager::AngryTimeout);
}

void PetTimerManager::StartIdleAnimation(int interval)
{
    IdleTimer->start(interval);
}

void PetTimerManager::StopIdleAnimation()
{
    IdleTimer->stop();
}

void PetTimerManager::StartRandomTalk(int minTime, int maxTime)
{
    int nextTime = QRandomGenerator::global()->bounded(minTime, maxTime);
    TalkTimer->start(nextTime);
}

void PetTimerManager::ResetSleepTimer(int interval)
{
    SleepTimer->start(interval);
}

void PetTimerManager::StartAngryTimer(int interval)
{
    AngryTimer->start(interval);
}