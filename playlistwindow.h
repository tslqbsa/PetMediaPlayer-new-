#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>

class PlaylistWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWindow(QWidget *parent = nullptr);

    // 设置播放列表
    void SetMusicList(const QStringList &MusicPaths);

    // 设置当前播放歌曲
    void SetCurrentMusicPath(const QString &MusicPath);

signals:
    void MusicDoubleClicked(int Index);// 之后双击歌曲时通知外部切歌
    void BackRequested(); // 请求返回主播放器窗口

private:
    QListWidget *MusicListWidget;

    QStringList CurrentMusicPaths;

    void InitUi();

    QLabel *TitleLabel;

    QLineEdit *SearchLineEdit;
    void UpdateMusicListByKeyword(const QString &Keyword);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // PLAYLISTWINDOW_H