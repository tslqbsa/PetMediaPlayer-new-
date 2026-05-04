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

private:
    QList<LyricLine> Lyrics;
};

#endif // LYRICMANAGER_H