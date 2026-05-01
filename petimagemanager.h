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

private:
    QStringList IdleImages;
    int IdleIndex;

    QString AngryPath;
    QString DragPath;
};

#endif // PETIMAGEMANAGER_H