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