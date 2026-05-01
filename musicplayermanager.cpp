#include "musicplayermanager.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDebug>

MusicPlayerManager::MusicPlayerManager(QObject *parent)
    : QObject(parent)
{
    Player = new QMediaPlayer(this);      // 创建播放器
    AudioOutput = new QAudioOutput(this); // 创建音频输出

    Player->setAudioOutput(AudioOutput);  // 连接音频输出

    AudioOutput->setVolume(1.0);          // 默认音量 100%

    qDebug() << "当前音量:" << AudioOutput->volume();

    connect(Player, &QMediaPlayer::mediaStatusChanged, this,
            [](QMediaPlayer::MediaStatus status) {
                qDebug() << "媒体状态:" << status;
            });

    connect(Player, &QMediaPlayer::playbackStateChanged, this,
            [](QMediaPlayer::PlaybackState state) {
                qDebug() << "播放状态:" << state;
            });

    connect(Player, &QMediaPlayer::errorOccurred, this,
            [](QMediaPlayer::Error error, const QString &errorString) {
                qDebug() << "播放错误:" << error << errorString;
            });
}

void MusicPlayerManager::SetMusicFile(const QString &filePath)
{
    qDebug() << "加载文件:" << filePath;

    Player->stop(); // 切歌前先停止
    Player->setSource(QUrl::fromLocalFile(filePath));
}

void MusicPlayerManager::Play()
{
    qDebug() << "开始播放";

    Player->play();
}

void MusicPlayerManager::Pause()
{
    Player->pause();
}

void MusicPlayerManager::Stop()
{
    Player->stop();
}