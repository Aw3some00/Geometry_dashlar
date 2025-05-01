#include "settingsdialog.h"
#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Settings");
    setMinimumSize(400, 300);

    QSettings settings("MyCompany", "RhythmRunner");
    theme = new Theme();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // Display Mode
    QLabel* displayModeLabel = new QLabel("Display Mode:", this);
    layout->addWidget(displayModeLabel);
    displayModeComboBox = new QComboBox(this);
    displayModeComboBox->addItem("Windowed");
    displayModeComboBox->addItem("Fullscreen");
    int displayMode = settings.value("displayMode", 0).toInt();
    displayModeComboBox->setCurrentIndex(displayMode);
    layout->addWidget(displayModeComboBox);
    connect(displayModeComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &SettingsDialog::updateDisplayMode);

    // Mute/Unmute Button
    QHBoxLayout* soundLayout = new QHBoxLayout();
    QLabel* soundLabel = new QLabel("Sound:", this);
    muteButton = new QPushButton("Mute", this);
    muteButton->setCheckable(true);
    bool isMuted = settings.value("muted", false).toBool();
    muteButton->setChecked(isMuted);
    muteButton->setText(isMuted ? "Unmute" : "Mute");
    connect(muteButton, &QPushButton::clicked, this, &SettingsDialog::toggleMute);
    soundLayout->addWidget(soundLabel);
    soundLayout->addWidget(muteButton);
    layout->addLayout(soundLayout);

    // Color Theme
    QLabel* themeLabel = new QLabel("Color Theme:", this);
    layout->addWidget(themeLabel);
    themeComboBox = new QComboBox(this);
    themeComboBox->addItem("Dark");
    themeComboBox->addItem("Light");
    themeComboBox->addItem("Neon");
    int themeIndex = settings.value("theme", 0).toInt();
    themeComboBox->setCurrentIndex(themeIndex);
    layout->addWidget(themeComboBox);
    connect(themeComboBox, &QComboBox::currentIndexChanged, this, &SettingsDialog::updateTheme);

    // Spacer to push buttons to the bottom
    layout->addStretch();

    // Button layout for OK and Reset
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);
    buttonLayout->setAlignment(Qt::AlignCenter);

    // Reset Settings Button
    resetButton = new QPushButton("Reset Settings", this);
    resetButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonLayout->addWidget(resetButton);
    connect(resetButton, &QPushButton::clicked, this, &SettingsDialog::resetSettings);

    // OK Button
    okButton = new QPushButton("OK", this);
    okButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonLayout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::accept);

    layout->addLayout(buttonLayout);

    // Apply the initial theme
    updateTheme(themeIndex);
}

void SettingsDialog::updateDisplayMode(int index) {
    QSettings settings("MyCompany", "RhythmRunner");
    settings.setValue("displayMode", index);

    QWidget* mainWindow = parentWidget()->window();
    bool isFullScreen = false;
    switch (index) {
    case 0: // Windowed
        mainWindow->setWindowFlags(Qt::Window);
        mainWindow->showNormal();
        mainWindow->resize(1280, 720);
        isFullScreen = false;
        break;
    case 1: // Fullscreen
        mainWindow->showFullScreen();
        isFullScreen = true;
        break;
    default:
        mainWindow->showNormal();
        mainWindow->resize(1280, 720);
        isFullScreen = false;
    }

    emit displayModeChanged(isFullScreen);
}

void SettingsDialog::toggleMute() {
    QSettings settings("MyCompany", "RhythmRunner");
    bool isMuted = muteButton->isChecked();
    settings.setValue("muted", isMuted);
    muteButton->setText(isMuted ? "Unmute" : "Mute");
}

void SettingsDialog::updateTheme(int index) {
    QSettings settings("MyCompany", "RhythmRunner");
    settings.setValue("theme", index);
    bool isFullScreen = parentWidget()->window()->isFullScreen();
    theme->applyTheme(this, index, resetButton, okButton, isFullScreen);

    MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget()->window());
    if (mainWindow) {
        mainWindow->applyTheme(index);
    } else {
        qWarning() << "Failed to get MainWindow for theme update";
    }
}

void SettingsDialog::resetSettings() {
    QSettings settings("MyCompany", "RhythmRunner");
    settings.clear();

    displayModeComboBox->setCurrentIndex(0);
    muteButton->setChecked(false);
    muteButton->setText("Mute");
    themeComboBox->setCurrentIndex(0);

    updateDisplayMode(0);
    toggleMute(); // Save the mute state
    updateTheme(0);

    QMessageBox::information(this, "Reset", "Settings have been reset to default.");
}
