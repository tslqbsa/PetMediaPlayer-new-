#include "lyricmanager.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

LyricManager::LyricManager()
{
}

void LyricManager::LoadLyrics(const QString &filePath)
{
    Lyrics.clear();

    QString LrcPath = filePath;
    LrcPath.replace(".mp3", ".lrc");
    LrcPath.replace(".wav", ".lrc");

    QFile File(LrcPath);

    if (!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "未找到歌词文件:" << LrcPath;
        return;
    }

    QTextStream In(&File);

    while (!In.atEnd()) {
        QString Line = In.readLine();

        QRegularExpression Rx(R"(\[(\d+):(\d+\.\d+)\])");

        QRegularExpressionMatchIterator It = Rx.globalMatch(Line);

        QString Text = Line;
        Text.remove(Rx); // 去掉所有时间标签

        while (It.hasNext()) {
            QRegularExpressionMatch Match = It.next();

            int Min = Match.captured(1).toInt();
            double Sec = Match.captured(2).toDouble();

            qint64 Time = (Min * 60 + Sec) * 1000;

            Lyrics.append({Time, Text});
        }
    }
}

QString LyricManager::GetCurrentLyric(qint64 position) const
{
    for (int i = Lyrics.size() - 1; i >= 0; --i) {
        if (position >= Lyrics[i].time) {
            return Lyrics[i].text;
        }
    }
    return "";
}

qint64 LyricManager::GetNextLyricTime(qint64 position) const
{
    for (int i = 0; i < Lyrics.size(); ++i) {
        if (position < Lyrics[i].time) {
            return Lyrics[i].time;
        }
    }
    return -1; // 没有下一句
}
QList<LyricLine> LyricManager::GetAllLyrics() const
{
    return Lyrics;
}
int LyricManager::GetCurrentLyricIndex(qint64 position) const
{
    for (int i = Lyrics.size() - 1; i >= 0; --i) {
        if (position >= Lyrics[i].time) {
            return i;
        }
    }

    return -1;
}