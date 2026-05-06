#include "traymanager.h"
#include "petwidget.h"

#include <QApplication>

TrayManager::TrayManager(PetWidget *petWidget, QObject *parent)
    : QObject(parent)
{
    Pet = petWidget;

    // 创建托盘图标
    TrayIcon = new QSystemTrayIcon(this);

    TrayIcon->setIcon(QIcon(":/resources/images/tray.png"));

    // 创建菜单
    TrayMenu = new QMenu();

    ShowAction = new QAction("显示桌宠", this);
    HideAction = new QAction("隐藏桌宠", this);
    SleepAction = new QAction("睡觉", this);
    WakeAction = new QAction("唤醒", this);
    SettingAction = new QAction("菜单", this);
    QuitAction = new QAction("退出", this);

    TrayMenu->addAction(ShowAction);
    TrayMenu->addAction(HideAction);
    TrayMenu->addSeparator();
    TrayMenu->addAction(SleepAction);
    TrayMenu->addAction(WakeAction);
    TrayMenu->addSeparator();
    TrayMenu->addAction(SettingAction);
    TrayMenu->addAction(QuitAction);

    TrayIcon->setContextMenu(TrayMenu);

    // 显示桌宠
    connect(ShowAction, &QAction::triggered,
            this, [this]() {

                Pet->show();
            });

    // 隐藏桌宠
    connect(HideAction, &QAction::triggered,
            this, [this]() {

                Pet->hide();
            });

    // 退出程序
    connect(QuitAction, &QAction::triggered,
            this, []() {

                QApplication::quit();
            });

    //睡觉
    connect(SleepAction, &QAction::triggered, this, [this]() {
        Pet->Sleep();
    });

    //唤醒
    connect(WakeAction, &QAction::triggered, this, [this]() {
        Pet->Wake();
    });

    //设置
    connect(SettingAction, &QAction::triggered, this, [this]() {
        Pet->OpenControlPanel();
    });

    // 双击托盘切换显示
    connect(TrayIcon, &QSystemTrayIcon::activated,
            this,
            [this](QSystemTrayIcon::ActivationReason reason) {

                if (reason == QSystemTrayIcon::DoubleClick) {

                    if (Pet->isVisible()) {
                        Pet->hide();
                    }
                    else {
                        Pet->show();
                    }
                }
            });

    TrayIcon->show();
}