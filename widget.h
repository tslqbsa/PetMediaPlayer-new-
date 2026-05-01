#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MusicPlayerManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_SelectMusicButton_clicked();

    void on_PlayButton_clicked();

    void on_PauseButton_clicked();

    void on_StopButton_clicked();

private:
    Ui::Widget *ui;

    MusicPlayerManager *MusicPlayer;
};

#endif // WIDGET_H