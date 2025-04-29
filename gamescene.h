#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include <QSoundEffect>
#include <QPushButton>
#include "NeonCityTrack.h"

class GameScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();
    void startGame();
    void setTrack(int trackId);
    bool isOnPlatform = false;
    bool isGameOver = false;
    void setupMusic();
    void stopMusic();
    void startMusic();
        QSoundEffect* musicEffect;
    QMediaPlayer *musicPlayer = nullptr;
signals:
    void returnToMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void update();
    void restartGame();

private:

    Player *player;
    QTimer *gameTimer;
    QPushButton *retryButton;
    QPushButton *exitButton;
    NeonCityTrack *currentTrack; // Используем NeonCityTrack напрямую
    int obstacleSpawnTimer = 0;
    int gameTime = 0; // Track time in frames
};

#endif // GAMESCENE_H
