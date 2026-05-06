
#include "playlistwindow.h"
#include "uistylehelper.h"

#include <QFileInfo>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>

PlaylistWindow::PlaylistWindow(QWidget *parent)
    : QWidget(parent)
{
    InitUi();
}

void PlaylistWindow::InitUi()
{
    setWindowTitle("播放列表");
    resize(320, 480);

    // 使用 UIStyleHelper 设置整体背景
    setStyleSheet(UIStyleHelper::GetPlaylistWindowStyle());

    // 返回按钮
    QPushButton *BackButton = new QPushButton("返回", this);
    BackButton->setStyleSheet(UIStyleHelper::GetPlaylistBackButtonStyle());

    // 标题
    TitleLabel = new QLabel("播放列表", this);
    TitleLabel->setAlignment(Qt::AlignCenter);
    TitleLabel->setStyleSheet(UIStyleHelper::GetPlaylistTitleLabelStyle());

    // 搜索框
    SearchLineEdit = new QLineEdit(this);
    SearchLineEdit->setPlaceholderText("搜索歌曲...");
    SearchLineEdit->setStyleSheet(UIStyleHelper::GetPlaylistSearchEditStyle());

    // 音乐列表
    MusicListWidget = new QListWidget(this);
    MusicListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    MusicListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MusicListWidget->setStyleSheet(UIStyleHelper::GetPlaylistListStyle());

    // 主布局
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->setContentsMargins(8, 8, 8, 8);
    MainLayout->addWidget(BackButton);
    MainLayout->addWidget(TitleLabel);
    MainLayout->addWidget(SearchLineEdit);
    MainLayout->addWidget(MusicListWidget, 1);

    // 双击歌曲，通知外部播放对应下标
    connect(MusicListWidget, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem *Item) {
                int Index = Item->data(Qt::UserRole).toInt();

                if (Index < 0 || Index >= CurrentMusicPaths.size()) {
                    return;
                }

                emit MusicDoubleClicked(Index);
            });

    // 点击返回按钮
    connect(BackButton, &QPushButton::clicked,
            this, [this]() {
                emit BackRequested();
            });


    connect(SearchLineEdit, &QLineEdit::textChanged,
            this, [this](const QString &Text) {
                UpdateMusicListByKeyword(Text);
            });
}


void PlaylistWindow::SetMusicList(const QStringList &MusicPaths)
{
    CurrentMusicPaths = MusicPaths;


    TitleLabel->setText(QString("播放列表（共 %1 首）").arg(CurrentMusicPaths.size()));


    if (SearchLineEdit != nullptr) {
        SearchLineEdit->clear();
    }


    UpdateMusicListByKeyword("");
}

void PlaylistWindow::UpdateMusicListByKeyword(const QString &Keyword)
{
    MusicListWidget->clear();

    // 没有歌曲
    if (CurrentMusicPaths.isEmpty()) {
        QListWidgetItem *Item = new QListWidgetItem("当前播放列表为空\n请先选择音乐文件夹");
        Item->setTextAlignment(Qt::AlignCenter);
        Item->setFlags(Item->flags() & ~Qt::ItemIsSelectable);

        MusicListWidget->addItem(Item);
        return;
    }

    QString LowerKeyword = Keyword.toLower();
    int ShowCount = 0;

    for (int i = 0; i < CurrentMusicPaths.size(); ++i) {
        QFileInfo FileInfo(CurrentMusicPaths[i]);
        QString SongName = FileInfo.baseName();
        if (!LowerKeyword.isEmpty() &&
            !SongName.toLower().contains(LowerKeyword)) {
            continue;
        }

        QListWidgetItem *Item = new QListWidgetItem(SongName);
        Item->setToolTip(CurrentMusicPaths[i]);

        Item->setData(Qt::UserRole, i);

        MusicListWidget->addItem(Item);
        ShowCount++;
    }

    if (ShowCount == 0) {
        QListWidgetItem *Item = new QListWidgetItem("没有找到匹配歌曲");
        Item->setTextAlignment(Qt::AlignCenter);
        Item->setFlags(Item->flags() & ~Qt::ItemIsSelectable);

        MusicListWidget->addItem(Item);
    }
}

void PlaylistWindow::SetCurrentMusicPath(const QString &MusicPath)
{
    for (int i = 0; i < MusicListWidget->count(); ++i) {
        QListWidgetItem *Item = MusicListWidget->item(i);

        if (Item == nullptr) {
            continue;
        }

        int RealIndex = Item->data(Qt::UserRole).toInt();

        if (RealIndex < 0 || RealIndex >= CurrentMusicPaths.size()) {
            continue;
        }

        QFileInfo FileInfo(CurrentMusicPaths[RealIndex]);
        QString DisplayName = FileInfo.baseName();

        if (CurrentMusicPaths[RealIndex] == MusicPath) {
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