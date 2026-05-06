#ifndef MUSICPLAYERMANAGER_H
#define MUSICPLAYERMANAGER_H

#include <QObject>
#include <QStringList>
#include <QList>
#include "lyricmanager.h"

class QMediaPlayer;
class QAudioOutput;

enum class PlayMode
{
    Sequential, // 顺序播放
    Loop,       // 单曲循环
    Random      // 随机播放
};

class MusicPlayerManager : public QObject
{
    Q_OBJECT
signals:
    void CurrentMusicChanged(); // 当前音乐改变
    void MusicStarted(); // 音乐开始播放
    void MusicPaused();  // 音乐暂停
    void MusicStopped(); // 音乐停止
    void PositionChanged(qint64 position); // 当前播放位置变化
    void DurationChanged(qint64 duration); // 音乐总时长变化
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
    void ChangePlayMode(); // 切换播放模式
    QString GetPlayModeText() const; // 获取播放模式
    void SetPosition(qint64 position); // 设置播放位置
    QString GetCurrentLyric(qint64 position) const;// 获取歌词
    qint64 GetNextLyricTime(qint64 position) const; //获取歌词时间
    QList<LyricLine> GetAllLyrics() const; // 获取全部歌词
    int GetCurrentLyricIndex(qint64 position) const; // 获取当前歌词下标
    QStringList GetMusicList() const;//获取音乐列表
    void PlayMusicByIndex(int index); // 根据下标播放歌曲
    void SetCurrentIndexByFilePath(const QString &filePath);


private:
    QMediaPlayer *Player;      // 音乐播放器
    QAudioOutput *AudioOutput; // 音频输出
    QStringList MusicList; // 音乐文件路径列表
    int CurrentIndex;      // 当前播放下标
    PlayMode CurrentPlayMode; // 当前播放模式
    QList<int> PlayHistory; // 播放历史
    QString CurrentFilePath; // 当前音乐文件路径
    LyricManager Lyric;//歌词
};

#endif // MUSICPLAYERMANAGER_H