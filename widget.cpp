#include "widget.h"
#include "musicplayermanager.h"
#include "./ui_widget.h"
#include "uistylehelper.h"
#include "petwidget.h"
#include "playlistwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QStyle>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QBrush>
#include <QFont>
#include <QColor>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setStyleSheet(UIStyleHelper::GetMainWindowStyle());

    ui->LyricListWidget->setStyleSheet(UIStyleHelper::GetLyricListStyle());
    ui->LyricListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->LyricListWidget->setWordWrap(true);
    ui->LyricListWidget->setTextElideMode(Qt::ElideNone);

    // 歌词列表滚动更顺滑
    ui->LyricListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // 歌词尺寸变化时自动调整
    ui->LyricListWidget->setResizeMode(QListView::Adjust);

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

    QString PushButtonStyle = UIStyleHelper::GetPushButtonStyle();

    ui->SelectMusicButton->setStyleSheet(PushButtonStyle);
    ui->SelectFolderButton->setStyleSheet(PushButtonStyle);
    ui->PlayModeButton->setStyleSheet(PushButtonStyle);
    ui->PlaylistButton->setStyleSheet(PushButtonStyle);

    //初始化进度条
    CurrentDuration = 0;
    IsProgressSliderPressed = false;

    LastLyric = "";

    MusicPlayer = new MusicPlayerManager(this);

    connect(MusicPlayer, &MusicPlayerManager::CurrentMusicChanged,
            this, [this]() {
                UpdateCurrentMusicLabel();

                LastLyric = "";
                LastLyricIndex = -1;

                UpdateLyricList();

                if (Playlist != nullptr) {
                    Playlist->SetCurrentMusicPath(MusicPlayer->GetCurrentFilePath());
                }
            });
    this->hide();

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

                    qint64 nextTime = MusicPlayer->GetNextLyricTime(position);

                    int duration = 4000; // 默认

                    if (nextTime > 0) {
                        duration = nextTime - position;

                        if (duration < 1000) duration = 1000; // 最短1秒
                        if (duration > 8000) duration = 8000; // 最长8秒
                    }

                    Pet->ShowLyric(lyric, duration);
                }
                int LyricIndex = MusicPlayer->GetCurrentLyricIndex(position);

                if (LyricIndex != LastLyricIndex &&
                    LyricIndex >= 0 &&
                    LyricIndex < ui->LyricListWidget->count()) {

                    LastLyricIndex = LyricIndex;
                    UpdateLyricHighlight(LyricIndex);
                }
            });

    connect(MusicPlayer, &MusicPlayerManager::DurationChanged,
            this, [this](qint64 duration) {
                CurrentDuration = duration;
                ui->ProgressSlider->setMaximum(duration);
                ui->TimeLabel->setText("00:00 / " + FormatTime(CurrentDuration));
            });

    //初始化列表
    Playlist = new PlaylistWindow(nullptr);
    Playlist->hide();

    connect(Playlist, &PlaylistWindow::MusicDoubleClicked,
            this, [this](int Index) {
                MusicPlayer->PlayMusicByIndex(Index);
            });
    connect(Playlist, &PlaylistWindow::BackRequested,
            this, [this]() {
                Playlist->hide();
                this->show();
                this->raise();
                this->activateWindow();
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

    Playlist->SetMusicList(MusicPlayer->GetMusicList());
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

void Widget::UpdateLyricList()
{
    ui->LyricListWidget->clear();

    QList<LyricLine> Lyrics = MusicPlayer->GetAllLyrics();

    if (Lyrics.isEmpty()) {
        QListWidgetItem *Item = new QListWidgetItem("暂无歌词");
        Item->setTextAlignment(Qt::AlignCenter);
        Item->setForeground(QBrush(QColor(150, 150, 150)));
        Item->setFont(QFont("Microsoft YaHei", 12));

        ui->LyricListWidget->addItem(Item);
        return;
    }

    for (const LyricLine &Line : Lyrics) {
        QListWidgetItem *Item = new QListWidgetItem(Line.text);

        Item->setTextAlignment(Qt::AlignCenter);
        Item->setForeground(QBrush(QColor(120, 120, 120)));
        Item->setFont(QFont("Microsoft YaHei", 9));

        ui->LyricListWidget->addItem(Item);
    }
}

void Widget::PauseMusic()
{
    MusicPlayer->Pause();
}

void Widget::UpdateLyricHighlight(int CurrentIndex)
{
    for (int i = 0; i < ui->LyricListWidget->count(); ++i) {
        QListWidgetItem *Item = ui->LyricListWidget->item(i);

        if (i == CurrentIndex) {
            Item->setTextAlignment(Qt::AlignCenter);
            Item->setForeground(QBrush(Qt::white));
            Item->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
        }
        else if (qAbs(i - CurrentIndex) <= 2) {
            Item->setTextAlignment(Qt::AlignCenter);
            Item->setForeground(QBrush(QColor(180, 180, 180)));
            Item->setFont(QFont("Microsoft YaHei", 10));
        }
        else {
            Item->setTextAlignment(Qt::AlignCenter);
            Item->setForeground(QBrush(QColor(120, 120, 120)));
            Item->setFont(QFont("Microsoft YaHei", 9));
        }
    }

    ui->LyricListWidget->scrollToItem(
        ui->LyricListWidget->item(CurrentIndex),
        QAbstractItemView::PositionAtCenter
        );
}

void Widget::on_PlaylistButton_clicked()
{
    Playlist->SetMusicList(MusicPlayer->GetMusicList());
    Playlist->SetCurrentMusicPath(MusicPlayer->GetCurrentFilePath());

    Playlist->show();
    Playlist->raise();
    Playlist->activateWindow();

    this->hide();
}

