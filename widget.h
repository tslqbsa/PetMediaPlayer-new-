#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

class MusicPlayerManager;
class PetWidget;

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
    void SetPetWidget(PetWidget *petWidget); // 设置桌宠对象

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

    void on_PlayModeButton_clicked();


    void on_ProgressSlider_sliderReleased();

    void on_ProgressSlider_sliderPressed();

private:
    Ui::Widget *ui;

    MusicPlayerManager *MusicPlayer;

    QString CurrentFolderPath; // 当前音乐文件夹路径

    void UpdateCurrentMusicLabel(); // 更新当前歌曲名显示

    PetWidget *Pet; // 桌宠对象

    qint64 CurrentDuration; // 当前音乐总时长
    QString FormatTime(qint64 ms); // 格式化时间

    bool IsProgressSliderPressed; // 是否正在拖动进度条
    QString LastLyric; // 上一次显示的歌词

    void UpdateLyricList(); // 更新歌词列表
};

#endif // WIDGET_H