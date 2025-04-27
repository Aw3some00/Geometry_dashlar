#include "mainmenu.h"
#include "settingsdialog.h"
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include<QApplication>
#include<QScreen>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    theme = new Theme();
    setupUi();
    QSettings settings("MyCompany", "Geometry_dashlar");
    applyTheme(settings.value("theme", 0).toInt());
}

void MainMenu::setupUi() {
    setObjectName("MainMenu");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Top bar with settings
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();

    settingsButton = new QPushButton("Settings", this);
    settingsButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    settingsButton->setMinimumSize(150, 50);
    connect(settingsButton, &QPushButton::clicked, this, &MainMenu::openSettings);
    topLayout->addWidget(settingsButton);

    mainLayout->addLayout(topLayout);

    // Title
    titleLabel = new QLabel("Rhythm Runner", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Track selection
    trackLabel = new QLabel("Choose Your Track", this);
    trackLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(trackLabel);

    // Track buttons
    track1Button = createStyledButton("Neon City", 0);
    connect(track1Button, &QPushButton::clicked, this, [this]() { emit startGame(1); });
    mainLayout->addWidget(track1Button, 0, Qt::AlignCenter);

    track2Button = createStyledButton("Track 2: Coming Soon", 0);
    track2Button->setEnabled(false);
    mainLayout->addWidget(track2Button, 0, Qt::AlignCenter);

    track3Button = createStyledButton("Track 3: Coming Soon", 0);
    track3Button->setEnabled(false);
    mainLayout->addWidget(track3Button, 0, Qt::AlignCenter);

    mainLayout->addStretch();
}

QPushButton* MainMenu::createStyledButton(const QString& text, int themeIndex) {
    QPushButton* button = new QPushButton(text, this);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setMinimumSize(200, 60);
    button->setStyleSheet(theme->getButtonStyle(themeIndex, window()->isFullScreen()));
    return button;
}

void MainMenu::applyTheme(int themeIndex, bool isFullScreen) {
    if (themeIndex < 0 || themeIndex > 2) {
        themeIndex = 0;
    }

    setStyleSheet(QString("QWidget#MainMenu { %1 }").arg(theme->getMainMenuStyle(themeIndex)));

    QString buttonStyle = theme->getButtonStyle(themeIndex, isFullScreen);
    settingsButton->setStyleSheet(buttonStyle);
    track1Button->setStyleSheet(buttonStyle);
    track2Button->setStyleSheet(buttonStyle);
    track3Button->setStyleSheet(buttonStyle);

    QString labelStyle = theme->getLabelStyle(themeIndex);
    int titleFontSize = isFullScreen ? 56 : 48;
    int trackFontSize = isFullScreen ? 28 : 24;
    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        titleFontSize += 4;
        trackFontSize += 2;
    }
    titleLabel->setStyleSheet(QString("font-size: %1px; font-weight: bold; %2")
                                  .arg(titleFontSize)
                                  .arg(labelStyle));
    trackLabel->setStyleSheet(QString("font-size: %1px; %2")
                                  .arg(trackFontSize)
                                  .arg(labelStyle));
}

void MainMenu::openSettings() {
    SettingsDialog dialog(this);
    connect(&dialog, &SettingsDialog::displayModeChanged, this, [this](bool isFullScreen) {
        QSettings settings("MyCompany", "RhythmRunner");
        applyTheme(settings.value("theme", 0).toInt(), isFullScreen);
    });
    dialog.exec();
    QSettings settings("MyCompany", "RhythmRunner");
    applyTheme(settings.value("theme", 0).toInt(), window()->isFullScreen());
}
