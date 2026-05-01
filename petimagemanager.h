#ifndef PETIMAGEMANAGER_H
#define PETIMAGEMANAGER_H

#include <QString>
#include <QStringList>

class PetImageManager
{
public:
    PetImageManager();

    QString BaseImage() const;

    QString NextIdleImage();
    QString AngryImage() const;
    QString DragImage() const;
    QString SleepImage() const;

    QString NextListenIdleImage();
    QString ListenAngryImage() const;

private:
    QStringList IdleImages;
    int IdleIndex;

    QStringList ListenIdleImages;
    int ListenIdleIndex;

    QString AngryPath;
    QString DragPath;
    QString SleepPath;
    QString ListenAngryPath;
};

#endif // PETIMAGEMANAGER_H