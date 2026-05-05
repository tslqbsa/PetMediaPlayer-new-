#include "playlistwindow.h"

#include <QFileInfo>
#include <QListWidgetItem>
#include <QPushButton>

PlaylistWindow::PlaylistWindow(QWidget *parent)
    : QWidget(parent)
{
    InitUi();
}

void PlaylistWindow::InitUi()
{
    setWindowTitle("播放列表");
    resize(320, 480);

    setStyleSheet("background-color: #181818;");

    QPushButton *BackButton = new QPushButton("返回", this);

    TitleLabel = new QLabel("播放列表", this);
    TitleLabel->setAlignment(Qt::AlignCenter);
    TitleLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");

    MusicListWidget = new QListWidget(this);


    MusicListWidget->setStyleSheet(R"(
        QListWidget {
            background-color: #202020;
            color: #dddddd;
            border: none;
            font-size: 14px;
        }

        QListWidget::item {
            padding: 8px;
        }

        QListWidget::item:selected {
            background-color: #3a3a3a;
            color: white;
            font-weight: bold;
        }
    )");

    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->setContentsMargins(8, 8, 8, 8);
    MainLayout->addWidget(BackButton);
    MainLayout->addWidget(TitleLabel);
    MainLayout->addWidget(MusicListWidget, 1);

    connect(MusicListWidget, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem *Item) {
                int Index = MusicListWidget->row(Item);

                if (Index < 0 || Index >= CurrentMusicPaths.size()) {
                    return;
                }

                emit MusicDoubleClicked(Index);
            });

    connect(BackButton, &QPushButton::clicked,
            this, [this]() {
                emit BackRequested();
            });
}

void PlaylistWindow::SetMusicList(const QStringList &MusicPaths)
{
    CurrentMusicPaths = MusicPaths;

    MusicListWidget->clear();

    TitleLabel->setText(QString("播放列表（共 %1 首）").arg(CurrentMusicPaths.size()));

    if (CurrentMusicPaths.isEmpty()) {
        QListWidgetItem *Item = new QListWidgetItem("当前播放列表为空");
        Item->setTextAlignment(Qt::AlignCenter);
        Item->setFlags(Item->flags() & ~Qt::ItemIsSelectable);
        MusicListWidget->addItem(Item);
        return;
    }

    for (const QString &MusicPath : CurrentMusicPaths) {
        QFileInfo FileInfo(MusicPath);

        QListWidgetItem *Item = new QListWidgetItem(FileInfo.baseName());
        Item->setToolTip(MusicPath);

        MusicListWidget->addItem(Item);
    }
}
void PlaylistWindow::SetCurrentMusicPath(const QString &MusicPath)
{
    for (int i = 0; i < CurrentMusicPaths.size(); ++i) {
        QFileInfo FileInfo(CurrentMusicPaths[i]);
        QString DisplayName = FileInfo.baseName();

        QListWidgetItem *Item = MusicListWidget->item(i);

        if (Item == nullptr) {
            continue;
        }

        if (CurrentMusicPaths[i] == MusicPath) {
            Item->setText("▶ " + DisplayName);
            MusicListWidget->setCurrentRow(i);
            MusicListWidget->scrollToItem(
                Item,
                QAbstractItemView::PositionAtCenter
                );
        }
        else {
            Item->setText(DisplayName);
        }
    }
}

void PlaylistWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
    emit BackRequested();
}