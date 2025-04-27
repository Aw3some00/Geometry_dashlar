#include "theme.h"
#include <QApplication>
#include <QScreen>

Theme::Theme() {
    // Enable high-DPI support
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

void Theme::applyTheme(QWidget* widget, int themeIndex, QPushButton* resetBtn, QPushButton* okBtn, bool isFullScreen) {
    switch (themeIndex) {
    case 0: setupDarkTheme(isFullScreen); break;
    case 1: setupLightTheme(isFullScreen); break;
    case 2: setupNeonTheme(isFullScreen); break;
    default: setupDarkTheme(isFullScreen);
    }

    widget->setStyleSheet(currentDialogStyle);

    if (resetBtn && okBtn) {
        resetBtn->setStyleSheet(getButtonStyle(themeIndex, isFullScreen));
        okBtn->setStyleSheet(getButtonStyle(themeIndex, isFullScreen));
    }
}

QString Theme::getButtonStyle(int themeIndex, bool isFullScreen) const {
    int fontSize = isFullScreen ? 25 : 20;
    QString padding = isFullScreen ? "15px 40px" : "12px 35px";
    int minWidth = isFullScreen ? 500 : 280;
    int minHeight = isFullScreen ? 40 : 30;

    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        fontSize += 2;
        minWidth += 40;
        minHeight += 10;
    }

    QString baseStyle = QString(
        "QPushButton {"
        "background: %1;"
        "color: %2;"
        "border: 2px solid %3;"
        "border-radius: 10px;"
        "padding: %4;"
        "font-size: %5px;"
        "font-weight: bold;"
        "min-width: %6px;"
        "min-height: %7px;"
        "}"
        "QPushButton:hover { "
        "   background: %8; "
        "   border: 2px solid %9; "
        "   padding: %4; "                // Maintain padding
        "   transition: all 0.3s ease; "  // Smooth transition
        "}"
        "QPushButton:pressed { "
        "   background: %10; "
        "   transform: scale(0.98); "     // Subtle press effect
        "}"
        "QPushButton:disabled { background: #666666; color: #cccccc; border: 2px solid #555555; }"
        );



    switch (themeIndex) {
    case 0: // Dark
        return baseStyle.arg(
            "#FF4444", "#FFFFFF", "#CC0000", // Background, text, border
            padding, QString::number(fontSize), QString::number(minWidth), QString::number(minHeight),
            "#FF6666", "#DD3333", "#CC2222" // Hover, hover border, pressed
            );
    case 1: // Light
        return baseStyle.arg(
            "#42A5F5", "#000000", "#1976D2",
            padding, QString::number(fontSize), QString::number(minWidth), QString::number(minHeight),
            "#64B5F6", "#2E86C1", "#1E88E5"
            );
    case 2: // Neon
        return baseStyle.arg(
            "#FF007F", "#E0F7FA", "#C60062",
            padding, QString::number(fontSize), QString::number(minWidth), QString::number(minHeight),
            "#FF3399", "#D40073", "#C60062"
            );
    default:
        return baseStyle.arg(
            "#FF4444", "#FFFFFF", "#CC0000",
            padding, QString::number(fontSize), QString::number(minWidth), QString::number(minHeight),
            "#FF6666", "#DD3333", "#CC2222"
            );
    }
}

QString Theme::getLabelStyle(int themeIndex) const {
    int fontSize = 16;
    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        fontSize += 2; // Adjust for high-DPI
    }

    QString baseStyle = QString("color: %1; font-size: %2px; font-family: Arial;");
    switch (themeIndex) {
    case 0: return baseStyle.arg("#FFFFFF", QString::number(fontSize)); // Dark
    case 1: return baseStyle.arg("#000000", QString::number(fontSize)); // Light
    case 2: return baseStyle.arg("#E0F7FA", QString::number(fontSize)); // Neon
    default: return baseStyle.arg("#FFFFFF", QString::number(fontSize));
    }
}

QString Theme::getDialogStyle(int themeIndex) const {
    switch (themeIndex) {
    case 0: return "QDialog { background: #1A1A1A; }"; // Softer dark
    case 1: return "QDialog { background: #F5F5F5; }"; // Softer light
    case 2: return "QDialog { background: #1A1A2E; }"; // Neon
    default: return "QDialog { background: #1A1A1A; }";
    }
}

QString Theme::getMainMenuStyle(int themeIndex) const {
    switch (themeIndex) {
    case 0: return "background-color: #1A1A1A;"; // Dark
    case 1: return "background-color: #F5F5F5;"; // Light
    case 2: return "background-color: #1A1A2E;"; // Neon (adjusted from bright #00FFEE)
    default: return "background-color: #1A1A1A;";
    }
}

void Theme::setupDarkTheme(bool isFullScreen) {
    int fontSize = isFullScreen ? 16 : 14;
    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        fontSize += 2;
    }

    currentDialogStyle = QString(
                             "QDialog { background: #1A1A1A; }"
                             "QComboBox { background: #333333; color: #FFFFFF; border: 1px solid #444444; padding: 8px; font-size: %1px; }"
                             "QComboBox::drop-down { border: none; }"
                             "QComboBox::down-arrow { image: none; width: 12px; height: 12px; }"
                             "QComboBox QAbstractItemView { background: #333333; color: #FFFFFF; selection-background-color: #FF4444; }"
                             "QComboBox:hover { background: #444444; }"
                             "QSlider::groove:horizontal { background: #444444; height: 8px; border-radius: 4px; }"
                             "QSlider::handle:horizontal { background: #FF4444; width: 20px; margin: -4px 0; border-radius: 10px; }"
                             "QSlider::handle:horizontal:hover { background: #FF6666; }"
                             ).arg(fontSize);

    currentButtonStyle = getButtonStyle(0, isFullScreen);
    currentLabelStyle = getLabelStyle(0);
}

void Theme::setupLightTheme(bool isFullScreen) {
    int fontSize = isFullScreen ? 16 : 14;
    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        fontSize += 2;
    }

    currentDialogStyle = QString(
                             "QDialog { background: #F5F5F5; color: #000000; }"
                             "QComboBox { background: #E3F2FD; color: #000000; border: 1px solid #BBDEFB; padding: 8px; font-size: %1px; }"
                             "QComboBox::drop-down { border: none; }"
                             "QComboBox::down-arrow { image: none; width: 12px; height: 12px; }"
                             "QComboBox QAbstractItemView { background: #E3F2FD; color: #000000; selection-background-color: #42A5F5; }"
                             "QComboBox:hover { background: #BBDEFB; }"
                             "QSlider::groove:horizontal { background: #E0E0E0; height: 8px; border-radius: 4px; }"
                             "QSlider::handle:horizontal { background: #42A5F5; width: 20px; margin: -4px 0; border-radius: 10px; }"
                             "QSlider::handle:horizontal:hover { background: #64B5F6; }"
                             ).arg(fontSize);

    currentButtonStyle = getButtonStyle(1, isFullScreen);
    currentLabelStyle = getLabelStyle(1);
}

void Theme::setupNeonTheme(bool isFullScreen) {
    int fontSize = isFullScreen ? 16 : 14;
    if (QApplication::primaryScreen()->logicalDotsPerInch() > 96) {
        fontSize += 2;
    }

    currentDialogStyle = QString(
                             "QDialog { background: #1A1A2E; }"
                             "QComboBox { background: #2A2A4A; color: #E0F7FA; border: 1px solid #00FFEE; padding: 8px; font-size: %1px; }"
                             "QComboBox::drop-down { border: none; }"
                             "QComboBox::down-arrow { image: none; width: 12px; height: 12px; }"
                             "QComboBox QAbstractItemView { background: #2A2A4A; color: #E0F7FA; selection-background-color: #FF007F; }"
                             "QComboBox:hover { background: #3A3A5A; }"
                             "QSlider::groove:horizontal { background: #00FFEE; height: 8px; border-radius: 4px; }"
                             "QSlider::handle:horizontal { background: #FF007F; width: 20px; margin: -4px 0; border-radius: 10px; }"
                             "QSlider::handle:horizontal:hover { background: #FF3399; }"
                             ).arg(fontSize);

    currentButtonStyle = getButtonStyle(2, isFullScreen);
    currentLabelStyle = getLabelStyle(2);
}
