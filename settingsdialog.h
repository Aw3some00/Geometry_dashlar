#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include "theme.h"

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

private slots:
    void updateDisplayMode(int index);
    void toggleMute();
    void updateTheme(int index);
    void resetSettings();

signals:
    void displayModeChanged(bool isFullScreen);

private:
    Theme* theme;
    QComboBox* displayModeComboBox;
    QComboBox* themeComboBox;
    QPushButton* muteButton; 
    QPushButton* resetButton;
    QPushButton* okButton;
};

#endif // SETTINGSDIALOG_H
