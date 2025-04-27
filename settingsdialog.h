#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSlider>
#include "theme.h"

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

private slots:
    void updateDisplayMode(int index);
    void updateVolume(int value);
    void updateTheme(int index);
    void resetSettings();

signals:
    void displayModeChanged(bool isFullScreen);

private:
    Theme* theme;
    QComboBox* displayModeComboBox;
    QSlider* volumeSlider;
    QComboBox* themeComboBox;
    QPushButton* resetButton;
    QPushButton* okButton;
};

#endif // SETTINGSDIALOG_H
