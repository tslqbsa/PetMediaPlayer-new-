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

private:
    QStringList IdleImages;
    int IdleIndex;

    QString AngryPath;
};

#endif // PETIMAGEMANAGER_H