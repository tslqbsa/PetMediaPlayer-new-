#include "widget.h"
#include "musicplayermanager.h"
#include "./ui_widget.h"
#include "petwidget.h"
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    MusicPlayer = new MusicPlayerManager(this);
    this->hide();
}

Widget::~Widget()
{
    delete ui;
}
void Widget::on_SelectMusicButton_clicked()
{
    QString FilePath = QFileDialog::getOpenFileName(
        this,
        "选择音乐",
        "",
        "Music Files (*.mp3 *.wav)"
        );

    if (FilePath.isEmpty()) {
        return; // 取消选择
    }

    MusicPlayer->SetMusicFile(FilePath); // 交给播放器管理器

    QFileInfo FileInfo(FilePath);//歌曲名
    QString FileName = FileInfo.fileName();
    qDebug() << "当前歌曲名:" << FileName;
    ui->CurrentMusicLabel->setText("当前播放：" + FileName);
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

