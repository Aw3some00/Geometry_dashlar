#include "mainmenu.h"
#include "settingsdialog.h"
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    theme = new Theme();
    setupUi();
    QSettings settings("MyCompany", "RhythmRunner");
    applyTheme(settings.value("theme", 0).toInt());
}

void MainMenu::setupUi() {
    setObjectName("MainMenu");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(20, 20, 20, 20);  // Increased margins
    mainLayout->setSpacing(25);

    // Top bar with settings
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();

    settingsButton = new QPushButton("Settings", this);
    settingsButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // Make size fixed
    settingsButton->setFixedSize(40, 16);  // Smaller size
    settingsButton->setFont(QFont("Arial", 9, QFont::Bold));  // Smaller font
    connect(settingsButton, &QPushButton::clicked, this, &MainMenu::openSettings);
    topLayout->addWidget(settingsButton);

    mainLayout->addLayout(topLayout);

    // Title
    titleLabel = new QLabel("Geometry Dashlar", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Track selection
    trackLabel = new QLabel("Choose Your Track", this);
    trackLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(trackLabel);

    // Track buttons
    track1Button = createStyledButton("Baku Nation", 0);
    connect(track1Button, &QPushButton::clicked, this, [this]() { emit startGame(1); });
    mainLayout->addWidget(track1Button, 0, Qt::AlignCenter);

    track2Button = createStyledButton("Track 2: Coming Soon", 0);
    track2Button->setEnabled(false);
    mainLayout->addWidget(track2Button, 0, Qt::AlignCenter);

    track3Button = createStyledButton("Track 3: Coming Soon", 0);
    track3Button->setEnabled(false);
    mainLayout->addWidget(track3Button, 0, Qt::AlignCenter);

    track1Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    track2Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    track3Button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Bottom layout for exit button (aligned to the right)
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(); // Push the exit button to the right

    exitButton = new QPushButton("Exit", this);
    exitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    exitButton->setFixedSize(40, 16); // Smaller size
    exitButton->setFont(QFont("Arial", 9, QFont::Bold)); // Smaller font
    connect(exitButton, &QPushButton::clicked, this, &MainMenu::exitApplication);
    bottomLayout->addWidget(exitButton);

    mainLayout->addLayout(bottomLayout); // Add bottom layout to main layout
}

QPushButton* MainMenu::createStyledButton(const QString& text, int themeIndex) {
    QPushButton* button = new QPushButton(text, this);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
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
    exitButton->setStyleSheet(buttonStyle); // Apply theme to exit button

    QString labelStyle = theme->getLabelStyle(themeIndex);
    int titleFontSize = isFullScreen ? 100 : 62;
    int trackFontSize = isFullScreen ? 95 : 54;

    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        titleFontSize += 8;
        trackFontSize += 4;
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

void MainMenu::exitApplication() {
    QApplication::quit(); // Close the application
}
