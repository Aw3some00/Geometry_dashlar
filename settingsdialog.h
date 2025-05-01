#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include "gamescene.h"
#include <QPushButton>
#include "theme.h"

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);
 explicit SettingsDialog(GameScene* gameScene, QWidget *parent = nullptr);
private slots:
    void updateDisplayMode(int index);

    void updateTheme(int index);
    void resetSettings();

signals:
    void displayModeChanged(bool isFullScreen);

private:
      GameScene* m_gameScene;
    Theme* theme;
    QComboBox* displayModeComboBox;
    QComboBox* themeComboBox;
    QPushButton* resetButton;
    QPushButton* okButton;
};

#endif // SETTINGSDIALOG_H
