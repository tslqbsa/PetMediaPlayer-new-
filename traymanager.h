#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
class PetWidget;
class TrayManager : public QObject
{
    Q_OBJECT

public:
    explicit TrayManager(PetWidget *petWidget, QObject *parent = nullptr);

private:
    PetWidget *Pet;

    QSystemTrayIcon *TrayIcon;

    QMenu *TrayMenu;

    QAction *ShowAction;
    QAction *HideAction;
    QAction *SleepAction;
    QAction *WakeAction;
    QAction *QuitAction;
};

#endif // TRAYMANAGER_H