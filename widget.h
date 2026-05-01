#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

class MusicPlayerManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_SelectMusicButton_clicked();

    void on_PlayButton_clicked();

    void on_PauseButton_clicked();

    void on_StopButton_clicked();

    void on_SelectFolderButton_clicked();

    void on_PreviousButton_clicked();

    void on_NextButton_clicked();

    void on_ForwardButton_clicked();

    void on_BackwardButton_clicked();

    void on_VolumeSlider_valueChanged(int value);

private:
    Ui::Widget *ui;

    MusicPlayerManager *MusicPlayer;

    QString CurrentFolderPath; // 当前音乐文件夹路径

    void UpdateCurrentMusicLabel(); // 更新当前歌曲名显示
};

#endif // WIDGET_H