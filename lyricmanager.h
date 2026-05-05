#ifndef LYRICMANAGER_H
#define LYRICMANAGER_H

#include <QString>
#include <QList>

struct LyricLine
{
    qint64 time;
    QString text;
};

class LyricManager
{
public:
    LyricManager();

    void LoadLyrics(const QString &filePath); // 加载歌词
    QString GetCurrentLyric(qint64 position) const; // 获取当前歌词
    qint64 GetNextLyricTime(qint64 position) const;
    QList<LyricLine> GetAllLyrics() const; // 获取全部歌词
    int GetCurrentLyricIndex(qint64 position) const; // 获取当前歌词下标

private:
    QList<LyricLine> Lyrics;
};

#endif // LYRICMANAGER_H