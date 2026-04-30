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