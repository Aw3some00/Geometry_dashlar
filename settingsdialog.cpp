#include "settingsdialog.h"
#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>

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
    //displayModeComboBox->addItem("Borderless");
    int displayMode = settings.value("displayMode", 0).toInt();
    displayModeComboBox->setCurrentIndex(displayMode);
    layout->addWidget(displayModeComboBox);
    connect(displayModeComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &SettingsDialog::updateDisplayMode);

    // Music Volume
    QLabel* volumeLabel = new QLabel("Music Volume:", this);
    layout->addWidget(volumeLabel);
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    int volume = settings.value("volume", 50).toInt();
    volumeSlider->setValue(volume);
    layout->addWidget(volumeSlider);
    connect(volumeSlider, &QSlider::valueChanged, this, &SettingsDialog::updateVolume);

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
    //connect(themeComboBox, &QComboBox::currentIndexChanged, this, &SettingsDialog::updateTheme);
    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::updateTheme);

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
   /* case 2: // Borderless
        mainWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        mainWindow->showMaximized();
        isFullScreen = true;
        break;*/
    default:
        mainWindow->showNormal();
        mainWindow->resize(1280, 720);
        isFullScreen = false;
    }

    emit displayModeChanged(isFullScreen);
}

void SettingsDialog::updateVolume(int value) {
    QSettings settings("MyCompany", "RhythmRunner");
    settings.setValue("volume", value);
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
    volumeSlider->setValue(50);
    themeComboBox->setCurrentIndex(0);

    updateDisplayMode(0);
    updateVolume(50);
    updateTheme(0);

    QMessageBox::information(this, "Reset", "Settings have been reset to default.");
}
