#include "uistylehelper.h"

QString UIStyleHelper::GetToolButtonStyle()
{
    return R"(
        QToolButton {
            border: none;
            background: transparent;
            padding: 5px;
        }

        QToolButton:hover {
            background-color: rgba(255, 255, 255, 30);
            border-radius: 8px;
        }

        QToolButton:pressed {
            background-color: rgba(255, 255, 255, 60);
        }
    )";
}
QString UIStyleHelper::GetMainWindowStyle()
{
    return R"(
        QWidget {
            background-color: #181818;
            color: white;
            font-family: "Microsoft YaHei";
        }
    )";
}

QString UIStyleHelper::GetLyricListStyle()
{
    return R"(
        QListWidget {
            background-color: #202020;
            color: #dddddd;
            border: 1px solid #333333;
            border-radius: 8px;
            padding: 6px;
        }

        QListWidget::item {
            padding: 6px;
        }
    )";
}

QString UIStyleHelper::GetSliderStyle()
{
    return R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #555555;
            border-radius: 3px;
        }

        QSlider::handle:horizontal {
            width: 14px;
            height: 14px;
            background: #3daee9;
            border-radius: 7px;
            margin: -4px 0;
        }
    )";
}
QString UIStyleHelper::GetPushButtonStyle()
{
    return R"(
        QPushButton {
            background-color: #2d2d2d;
            color: white;
            border: 1px solid #444444;
            border-radius: 8px;
            padding: 6px 12px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #3a3a3a;
        }

        QPushButton:pressed {
            background-color: #505050;
        }
    )";
}
QString UIStyleHelper::GetPlaylistWindowStyle()
{
    return "background-color: #181818;";
}

QString UIStyleHelper::GetPlaylistBackButtonStyle()
{
    return R"(
        QPushButton {
            background-color: #2d2d2d;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #3a3a3a;
        }

        QPushButton:pressed {
            background-color: #505050;
        }
    )";
}

QString UIStyleHelper::GetPlaylistTitleLabelStyle()
{
    return "color: white; font-size: 18px; font-weight: bold;";
}

QString UIStyleHelper::GetPlaylistSearchEditStyle()
{
    return R"(
        QLineEdit {
            background-color: #2d2d2d;
            color: white;
            border: 1px solid #444444;
            border-radius: 8px;
            padding: 8px;
            font-size: 14px;
        }
    )";
}

QString UIStyleHelper::GetPlaylistListStyle()
{
    return R"(
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
    )";
}