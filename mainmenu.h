#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include "theme.h"

class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);

public slots:
    void applyTheme(int themeIndex, bool isFullScreen = false);
    void openSettings();

signals:
    void startGame(int trackId);

private:
    void setupUi();
    QPushButton* createStyledButton(const QString& text, int themeIndex);

    Theme* theme;
    QLabel* titleLabel;
    QLabel* trackLabel;
    QPushButton* settingsButton;
    QPushButton* track1Button;
    QPushButton* track2Button;
    QPushButton* track3Button;
};

#endif // MAINMENU_H
