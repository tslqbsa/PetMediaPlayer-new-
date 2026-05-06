#include "settingswindow.h"

#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    InitUI();
    LoadSettings();
    InitConnections();
}

void SettingsWindow::InitUI()
{
    setWindowTitle("设置");
    setFixedSize(320, 300);

    LyricBubbleCheckBox = new QCheckBox("显示歌词气泡", this);
    LyricBubbleCheckBox->setChecked(true);

    AlwaysOnTopCheckBox = new QCheckBox("桌宠始终置顶", this);
    AlwaysOnTopCheckBox->setChecked(true);

    OpacitySlider = new QSlider(Qt::Horizontal, this);
    OpacitySlider->setRange(30, 100);
    OpacitySlider->setValue(100);

    OpacityValueLabel = new QLabel("100%", this);

    ScaleSlider = new QSlider(Qt::Horizontal, this);
    ScaleSlider->setRange(50, 150);
    ScaleSlider->setValue(100);

    ScaleValueLabel = new QLabel("100%", this);

    VolumeSlider = new QSlider(Qt::Horizontal, this);
    VolumeSlider->setRange(0, 100);
    VolumeSlider->setValue(50);

    VolumeValueLabel = new QLabel("50%", this);

    CloseButton = new QPushButton("关闭", this);

    QVBoxLayout *MainLayout = new QVBoxLayout(this);

    MainLayout->addWidget(LyricBubbleCheckBox);
    MainLayout->addWidget(AlwaysOnTopCheckBox);

    QHBoxLayout *OpacityLayout = new QHBoxLayout;
    OpacityLayout->addWidget(new QLabel("桌宠透明度", this));
    OpacityLayout->addWidget(OpacitySlider);
    OpacityLayout->addWidget(OpacityValueLabel);

    QHBoxLayout *ScaleLayout = new QHBoxLayout;
    ScaleLayout->addWidget(new QLabel("桌宠大小", this));
    ScaleLayout->addWidget(ScaleSlider);
    ScaleLayout->addWidget(ScaleValueLabel);

    QHBoxLayout *VolumeLayout = new QHBoxLayout;
    VolumeLayout->addWidget(new QLabel("默认音量", this));
    VolumeLayout->addWidget(VolumeSlider);
    VolumeLayout->addWidget(VolumeValueLabel);

    MainLayout->addLayout(OpacityLayout);
    MainLayout->addLayout(ScaleLayout);
    MainLayout->addLayout(VolumeLayout);

    MainLayout->addStretch();
    MainLayout->addWidget(CloseButton);

    setLayout(MainLayout);
}

void SettingsWindow::InitConnections()
{
    connect(LyricBubbleCheckBox, &QCheckBox::toggled,
            this, [this](bool visible) {
                SaveSettings();
                emit LyricBubbleVisibleChanged(visible);
            });

    connect(AlwaysOnTopCheckBox, &QCheckBox::toggled,
            this, [this](bool enabled) {
                SaveSettings();
                emit AlwaysOnTopChanged(enabled);
            });

    connect(OpacitySlider, &QSlider::valueChanged,
            this, [this](int value) {
                OpacityValueLabel->setText(QString::number(value) + "%");
                SaveSettings();
                emit PetOpacityChanged(value);
            });

    connect(ScaleSlider, &QSlider::valueChanged,
            this, [this](int value) {
                ScaleValueLabel->setText(QString::number(value) + "%");
                SaveSettings();
                emit PetScaleChanged(value);
            });

    connect(VolumeSlider, &QSlider::valueChanged,
            this, [this](int value) {
                VolumeValueLabel->setText(QString::number(value) + "%");
                SaveSettings();
                emit DefaultVolumeChanged(value);
            });

    connect(CloseButton, &QPushButton::clicked,
            this, &QWidget::hide);
}
void SettingsWindow::LoadSettings()
{
    QSettings Settings("LinXuanyu", "PetMediaPlayer");

    LyricBubbleCheckBox->setChecked(Settings.value("LyricBubbleVisible", true).toBool());
    AlwaysOnTopCheckBox->setChecked(Settings.value("AlwaysOnTop", true).toBool());

    OpacitySlider->setValue(Settings.value("PetOpacity", 100).toInt());
    ScaleSlider->setValue(Settings.value("PetScale", 100).toInt());
    VolumeSlider->setValue(Settings.value("DefaultVolume", 50).toInt());

    OpacityValueLabel->setText(QString::number(OpacitySlider->value()) + "%");
    ScaleValueLabel->setText(QString::number(ScaleSlider->value()) + "%");
    VolumeValueLabel->setText(QString::number(VolumeSlider->value()) + "%");
}
void SettingsWindow::SaveSettings()
{
    QSettings Settings("LinXuanyu", "PetMediaPlayer");

    Settings.setValue("LyricBubbleVisible", LyricBubbleCheckBox->isChecked());
    Settings.setValue("AlwaysOnTop", AlwaysOnTopCheckBox->isChecked());

    Settings.setValue("PetOpacity", OpacitySlider->value());
    Settings.setValue("PetScale", ScaleSlider->value());
    Settings.setValue("DefaultVolume", VolumeSlider->value());
}