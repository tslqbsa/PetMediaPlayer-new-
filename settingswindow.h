#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>


class QCheckBox;
class QSlider;
class QLabel;
class QPushButton;

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

signals:
    void LyricBubbleVisibleChanged(bool visible);
    void PetOpacityChanged(int value);
    void PetScaleChanged(int value);
    void AlwaysOnTopChanged(bool enabled);
    void DefaultVolumeChanged(int value);

private:
    QCheckBox *LyricBubbleCheckBox = nullptr;
    QCheckBox *AlwaysOnTopCheckBox = nullptr;

    QSlider *OpacitySlider = nullptr;
    QSlider *ScaleSlider = nullptr;
    QSlider *VolumeSlider = nullptr;

    QLabel *OpacityValueLabel = nullptr;
    QLabel *ScaleValueLabel = nullptr;
    QLabel *VolumeValueLabel = nullptr;

    QPushButton *CloseButton = nullptr;

private:
    void InitUI();
    void InitConnections();
    void LoadSettings();
    void SaveSettings();
};


#endif // SETTINGSWINDOW_H