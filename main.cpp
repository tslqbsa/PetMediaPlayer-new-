
#include "petwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    qputenv("QT_MEDIA_BACKEND", QByteArray("windows"));

    QApplication a(argc, argv);

    PetWidget pet;
    pet.show();

    return a.exec();
}