#include "petimagemanager.h"

PetImageManager::PetImageManager()
{
    IdleImages = {
        ":/resources/images/quiet.png",
        ":/resources/images/quiet.png",
        ":/resources/images/quiet2.png",
    };

    IdleIndex = 0;

    AngryPath = ":/resources/images/angry.png";
    DragPath= ":/resources/images/drag.png";
    SleepPath= ":/resources/images/sleep.png";
}

QString PetImageManager::BaseImage() const
{
    return IdleImages[0];
}

QString PetImageManager::NextIdleImage()
{
    IdleIndex = (IdleIndex + 1) % IdleImages.size();
    return IdleImages[IdleIndex];
}

QString PetImageManager::AngryImage() const
{
    return AngryPath;
}

QString PetImageManager::DragImage() const
{
    return DragPath;
}

QString PetImageManager::SleepImage() const
{
    return SleepPath;
}