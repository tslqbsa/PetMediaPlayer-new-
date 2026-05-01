#include "widget.h"
#include "musicplayermanager.h"
#include "./ui_widget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    MusicPlayer = new MusicPlayerManager(this);

    connect(MusicPlayer, &MusicPlayerManager::CurrentMusicChanged,
            this, &Widget::UpdateCurrentMusicLabel);
    this->hide();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_PlayButton_clicked()
{
    MusicPlayer->Play(); // 播放音乐
}


void Widget::on_PauseButton_clicked()
{
    MusicPlayer->Pause(); // 暂停音乐
}


void Widget::on_StopButton_clicked()
{
    MusicPlayer->Stop(); // 停止音乐
}

void Widget::on_SelectMusicButton_clicked()
{
    QString FilePath = QFileDialog::getOpenFileName(
        this,
        "选择音乐",
        CurrentFolderPath,
        "Music Files (*.mp3 *.wav)"
        );

    if (FilePath.isEmpty()) {
        return; // 取消选择
    }

    MusicPlayer->SetMusicFile(FilePath); // 交给播放器管理器
    UpdateCurrentMusicLabel();           // 更新当前歌曲名
}

void Widget::on_SelectFolderButton_clicked()
{
    QString FolderPath = QFileDialog::getExistingDirectory(
        this,
        "选择音乐文件夹"
        );

    if (FolderPath.isEmpty()) {
        return;
    }

    CurrentFolderPath = FolderPath;          // 记录当前文件夹
    MusicPlayer->SetMusicFolder(FolderPath); // 读取文件夹歌曲
    UpdateCurrentMusicLabel();               // 更新当前歌曲名
}

void Widget::on_PreviousButton_clicked()
{
    MusicPlayer->PreviousMusic(); // 上一首
    UpdateCurrentMusicLabel();    // 更新当前歌曲名
}

void Widget::on_NextButton_clicked()
{
    MusicPlayer->NextMusic();  // 下一首
    UpdateCurrentMusicLabel(); // 更新当前歌曲名
}

void Widget::UpdateCurrentMusicLabel()
{
    QString CurrentFilePath = MusicPlayer->GetCurrentFilePath();

    if (CurrentFilePath.isEmpty()) {
        ui->CurrentMusicLabel->setText("当前未选择音乐");
        return;
    }

    QFileInfo FileInfo(CurrentFilePath);
    ui->CurrentMusicLabel->setText(FileInfo.baseName());
}


void Widget::on_ForwardButton_clicked()
{
    MusicPlayer->Forward();  // 下一首
}


void Widget::on_BackwardButton_clicked()
{
    MusicPlayer->Backward();  // 下一首
}


void Widget::on_VolumeSlider_valueChanged(int value)
{
    MusicPlayer->SetVolume(value); // 设置音量
}

