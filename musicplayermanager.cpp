#include "musicplayermanager.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDebug>
#include <QDir>

MusicPlayerManager::MusicPlayerManager(QObject *parent)
    : QObject(parent)
{
    Player = new QMediaPlayer(this);      // 创建播放器
    AudioOutput = new QAudioOutput(this); // 创建音频输出

    Player->setAudioOutput(AudioOutput);  // 连接音频输出

    AudioOutput->setVolume(0.0);          // 默认音量 0%

    CurrentIndex = -1; // 当前默认没有歌曲
    CurrentPlayMode = PlayMode::Sequential; // 默认顺序播放

    qDebug() << "当前音量:" << AudioOutput->volume();

    connect(Player, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::EndOfMedia) {
                    qDebug() << "当前歌曲播放结束，自动播放下一首";

                    NextMusic();
                }
            });

    connect(Player, &QMediaPlayer::playbackStateChanged, this,
            [](QMediaPlayer::PlaybackState state) {
    });

    connect(Player, &QMediaPlayer::errorOccurred, this,
            [](QMediaPlayer::Error error, const QString &errorString) {
    });
}

void MusicPlayerManager::SetMusicFile(const QString &filePath)
{
    qDebug() << "加载文件:" << filePath;

    Player->stop(); // 切歌前先停止
    Player->setSource(QUrl::fromLocalFile(filePath));
    emit CurrentMusicChanged(); // 通知界面更新当前歌曲名
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

void MusicPlayerManager::SetMusicFolder(const QString &folderPath)
{
    QDir Dir(folderPath);

    QStringList Filters;
    Filters << "*.mp3" << "*.wav";

    QFileInfoList FileInfoList = Dir.entryInfoList(
        Filters,
        QDir::Files,
        QDir::Name
        );

    MusicList.clear();

    for (const QFileInfo &FileInfo : FileInfoList) {
        MusicList.append(FileInfo.absoluteFilePath());
    }

    if (MusicList.isEmpty()) {
        qDebug() << "文件夹中没有找到音乐文件";
        CurrentIndex = -1;
        return;
    }

    CurrentIndex = 0;

    qDebug() << "读取音乐数量:" << MusicList.size();
    qDebug() << "第一首音乐:" << MusicList[CurrentIndex];

    SetMusicFile(MusicList[CurrentIndex]); // 默认加载第一首
}
QString MusicPlayerManager::GetCurrentFilePath() const
{
    if (CurrentIndex < 0 || CurrentIndex >= MusicList.size()) {
        return "";
    }

    return MusicList[CurrentIndex];
}

void MusicPlayerManager::NextMusic()
{
    if (MusicList.isEmpty()) {
        qDebug() << "播放列表为空";
        return;
    }
    if (CurrentIndex >= 0) {
        PlayHistory.append(CurrentIndex);
    }

    switch (CurrentPlayMode)
    {
    case PlayMode::Sequential:

        CurrentIndex++;

        if (CurrentIndex >= MusicList.size()) {
            CurrentIndex = 0;
        }

        break;

    case PlayMode::Loop:

        // 单曲循环
        // 不改变 CurrentIndex

        break;

    case PlayMode::Random:

        CurrentIndex = rand() % MusicList.size();

        break;
    }

    qDebug() << "当前播放:" << MusicList[CurrentIndex];

    SetMusicFile(MusicList[CurrentIndex]);
    Play();
}

void MusicPlayerManager::PreviousMusic()
{
    if (PlayHistory.isEmpty()) {
        qDebug() << "没有上一首播放历史";
        return;
    }

    CurrentIndex = PlayHistory.takeLast();

    qDebug() << "回到上一首播放历史:" << MusicList[CurrentIndex];

    SetMusicFile(MusicList[CurrentIndex]);
    Play();
}

void MusicPlayerManager::Forward(int ms)
{
    qint64 NewPosition = Player->position() + ms;

    if (NewPosition > Player->duration()) {
        NewPosition = Player->duration();
    }

    Player->setPosition(NewPosition); // 快进
}

void MusicPlayerManager::Backward(int ms)
{
    qint64 NewPosition = Player->position() - ms;

    if (NewPosition < 0) {
        NewPosition = 0;
    }

    Player->setPosition(NewPosition); // 快退
}

void MusicPlayerManager::SetVolume(int volume)
{
    if (volume < 0) {
        volume = 0;
    }

    if (volume > 100) {
        volume = 100;
    }

    double RealVolume = volume / 100.0;

    AudioOutput->setVolume(RealVolume);

    qDebug() << "当前音量:" << volume;
}

void MusicPlayerManager::ChangePlayMode()
{
    switch (CurrentPlayMode)
    {
    case PlayMode::Sequential:
        CurrentPlayMode = PlayMode::Loop;
        qDebug() << "当前播放模式：单曲循环";
        break;

    case PlayMode::Loop:
        CurrentPlayMode = PlayMode::Random;
        qDebug() << "当前播放模式：随机播放";
        break;

    case PlayMode::Random:
        CurrentPlayMode = PlayMode::Sequential;
        qDebug() << "当前播放模式：顺序播放";
        break;
    }
}

QString MusicPlayerManager::GetPlayModeText() const
{
    switch (CurrentPlayMode)
    {
    case PlayMode::Sequential:
        return "顺序播放";

    case PlayMode::Loop:
        return "单曲循环";

    case PlayMode::Random:
        return "随机播放";
    }

    return "顺序播放";
}

