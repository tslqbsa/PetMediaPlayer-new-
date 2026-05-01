#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayManager : public QObject
{
    Q_OBJECT

public:
    explicit TrayManager(QWidget *petWidget,
                         QObject *parent = nullptr);

private:
    QWidget *PetWidget;

    QSystemTrayIcon *TrayIcon;

    QMenu *TrayMenu;

    QAction *ShowAction;
    QAction *HideAction;
    QAction *QuitAction;
};

#endif // TRAYMANAGER_H