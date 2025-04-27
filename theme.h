#ifndef THEME_H
#define THEME_H

#include <QWidget>
#include <QPushButton>

class Theme {
public:
    Theme();
    void applyTheme(QWidget* widget, int themeIndex, QPushButton* resetBtn, QPushButton* okBtn, bool isFullScreen = false);
    QString getMainMenuStyle(int themeIndex) const;
    QString getButtonStyle(int themeIndex, bool isFullScreen) const;
    QString getLabelStyle(int themeIndex) const;

private:
    QString getDialogStyle(int themeIndex) const;
    void setupDarkTheme(bool isFullScreen);
    void setupLightTheme(bool isFullScreen);
    void setupNeonTheme(bool isFullScreen);

    QString currentDialogStyle;
    QString currentButtonStyle;
    QString currentLabelStyle;
};

#endif // THEME_H
