#ifndef MUSICPLAYERMANAGER_H
#define MUSICPLAYERMANAGER_H

#include <QObject>
#include <QStringList>

class QMediaPlayer;
class QAudioOutput;

class MusicPlayerManager : public QObject
{
    Q_OBJECT
signals:
    void CurrentMusicChanged(); // 当前音乐改变
public:
    explicit MusicPlayerManager(QObject *parent = nullptr);

    void SetMusicFile(const QString &filePath); // 设置音乐文件
    void Play();                                // 播放音乐
    void Pause();                               // 暂停音乐
    void Stop();                                // 停止音乐
    void SetMusicFolder(const QString &folderPath); // 设置音乐文件夹
    void NextMusic();                               // 下一首
    void PreviousMusic();                           // 上一首
    void Forward(int ms = 10000);                   // 快进
    void Backward(int ms = 10000);                  // 快退
    void SetVolume(int volume);                     // 设置音量
    QString GetCurrentFilePath() const; // 获取当前音乐路径

private:
    QMediaPlayer *Player;      // 音乐播放器
    QAudioOutput *AudioOutput; // 音频输出
    QStringList MusicList; // 音乐文件路径列表
    int CurrentIndex;      // 当前播放下标

};

#endif // MUSICPLAYERMANAGER_H