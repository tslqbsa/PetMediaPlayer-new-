#include "traymanager.h"

#include <QApplication>

TrayManager::TrayManager(QWidget *petWidget,
                         QObject *parent)
    : QObject(parent)
{
    PetWidget = petWidget;

    // 创建托盘图标
    TrayIcon = new QSystemTrayIcon(this);

    TrayIcon->setIcon(QIcon(":/resources/images/quiet.png"));

    // 创建菜单
    TrayMenu = new QMenu();

    ShowAction = new QAction("显示桌宠", this);
    HideAction = new QAction("隐藏桌宠", this);
    QuitAction = new QAction("退出", this);

    TrayMenu->addAction(ShowAction);
    TrayMenu->addAction(HideAction);
    TrayMenu->addSeparator();
    TrayMenu->addAction(QuitAction);

    TrayIcon->setContextMenu(TrayMenu);

    // 显示桌宠
    connect(ShowAction, &QAction::triggered,
            this, [this]() {

                PetWidget->show();
            });

    // 隐藏桌宠
    connect(HideAction, &QAction::triggered,
            this, [this]() {

                PetWidget->hide();
            });

    // 退出程序
    connect(QuitAction, &QAction::triggered,
            this, []() {

                QApplication::quit();
            });

    // 双击托盘切换显示
    connect(TrayIcon, &QSystemTrayIcon::activated,
            this,
            [this](QSystemTrayIcon::ActivationReason reason) {

                if (reason == QSystemTrayIcon::DoubleClick) {

                    if (PetWidget->isVisible()) {
                        PetWidget->hide();
                    }
                    else {
                        PetWidget->show();
                    }
                }
            });

    TrayIcon->show();
}