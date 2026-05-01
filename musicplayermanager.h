#ifndef MUSICPLAYERMANAGER_H
#define MUSICPLAYERMANAGER_H

#include <QObject>

class QMediaPlayer;
class QAudioOutput;

class MusicPlayerManager : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayerManager(QObject *parent = nullptr);

    void SetMusicFile(const QString &filePath); // 设置音乐文件
    void Play();                                // 播放音乐
    void Pause();                               // 暂停音乐
    void Stop();                                // 停止音乐

private:
    QMediaPlayer *Player;      // 音乐播放器
    QAudioOutput *AudioOutput; // 音频输出
};

#endif // MUSICPLAYERMANAGER_H