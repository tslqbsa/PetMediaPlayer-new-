#include "widget.h"
#include "musicplayermanager.h"
#include "./ui_widget.h"
#include "uistylehelper.h"
#include "petwidget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QStyle>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    Pet = nullptr;
    // 设置播放器按钮图标
    ui->PlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->PauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->StopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    ui->PreviousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->NextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    ui->BackwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->ForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    //图标风格
    QString ButtonStyle = UIStyleHelper::GetToolButtonStyle();
    ui->PlayButton->setStyleSheet(ButtonStyle);
    ui->PauseButton->setStyleSheet(ButtonStyle);
    ui->StopButton->setStyleSheet(ButtonStyle);

    ui->PreviousButton->setStyleSheet(ButtonStyle);
    ui->NextButton->setStyleSheet(ButtonStyle);

    ui->BackwardButton->setStyleSheet(ButtonStyle);
    ui->ForwardButton->setStyleSheet(ButtonStyle);

    ui->CurrentMusicLabel->setAlignment(Qt::AlignCenter);
    ui->CurrentMusicLabel->setWordWrap(true);
    ui->CurrentMusicLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-size: 22px;
        font-weight: bold;
    }
)");

    //初始化进度条
    CurrentDuration = 0;
    IsProgressSliderPressed = false;

    LastLyric = "";

    MusicPlayer = new MusicPlayerManager(this);

    connect(MusicPlayer, &MusicPlayerManager::CurrentMusicChanged,
            this, &Widget::UpdateCurrentMusicLabel);
    this->hide();

    connect(MusicPlayer, &MusicPlayerManager::MusicStarted, this, [this]() {
        if (Pet != nullptr) {
            Pet->StartListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::MusicPaused, this, [this]() {
        if (Pet != nullptr) {
            Pet->StopListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::MusicStopped, this, [this]() {
        if (Pet != nullptr) {
            Pet->StopListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::MusicStarted, this, [this]() {
        ui->PlayerStatusIconLabel->setText("▶");

        if (Pet != nullptr) {
            Pet->StartListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::MusicPaused, this, [this]() {
        ui->PlayerStatusIconLabel->setText("⏸");

        if (Pet != nullptr) {
            Pet->StopListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::MusicStopped, this, [this]() {
        ui->PlayerStatusIconLabel->setText("⏹");

        if (Pet != nullptr) {
            Pet->StopListen();
        }
    });

    connect(MusicPlayer, &MusicPlayerManager::PositionChanged,
            this, [this](qint64 position) {
                if (!IsProgressSliderPressed) {
                    ui->ProgressSlider->setValue(position);
                }

                ui->TimeLabel->setText(
                    FormatTime(position) + " / " + FormatTime(CurrentDuration)
                    );

                QString lyric = MusicPlayer->GetCurrentLyric(position);

                if (!lyric.isEmpty() && lyric != LastLyric && Pet != nullptr) {
                    LastLyric = lyric;
                    Pet->ShowLyric(lyric);
                }
            });

    connect(MusicPlayer, &MusicPlayerManager::DurationChanged,
            this, [this](qint64 duration) {
                CurrentDuration = duration;
                ui->ProgressSlider->setMaximum(duration);
                ui->TimeLabel->setText("00:00 / " + FormatTime(CurrentDuration));
            });



}

Widget::~Widget()
{
    delete ui;
}
void Widget::on_PlayButton_clicked()
{
    MusicPlayer->Play();
}

void Widget::on_PauseButton_clicked()
{
    MusicPlayer->Pause();
}

void Widget::on_StopButton_clicked()
{
    MusicPlayer->Stop();
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
}

void Widget::on_NextButton_clicked()
{
    MusicPlayer->NextMusic(); // 下一首
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
    MusicPlayer->Forward();  // 快进
}

void Widget::on_BackwardButton_clicked()
{
    MusicPlayer->Backward(); // 快退
}


void Widget::on_VolumeSlider_valueChanged(int value)
{
    MusicPlayer->SetVolume(value); // 设置音量
}


void Widget::on_PlayModeButton_clicked()
{
    MusicPlayer->ChangePlayMode();
    ui->PlayModeButton->setText(MusicPlayer->GetPlayModeText());
}

void Widget::SetPetWidget(PetWidget *petWidget)
{
    Pet = petWidget;
}


QString Widget::FormatTime(qint64 ms)
{
    int TotalSeconds = ms / 1000;
    int Minutes = TotalSeconds / 60;
    int Seconds = TotalSeconds % 60;

    return QString("%1:%2")
        .arg(Minutes, 2, 10, QChar('0'))
        .arg(Seconds, 2, 10, QChar('0'));
}

void Widget::on_ProgressSlider_sliderPressed()
{
    IsProgressSliderPressed = true;
}

void Widget::on_ProgressSlider_sliderReleased()
{
    IsProgressSliderPressed = false;
    MusicPlayer->SetPosition(ui->ProgressSlider->value());
}

