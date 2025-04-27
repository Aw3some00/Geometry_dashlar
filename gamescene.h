#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include "obstacle.h"
#include <QMediaPlayer>
#include <QPushButton>
#include "NeonCityTrack.h"

class GameScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();
    void startGame();
    void setTrack(int trackId);
  //  QMediaPlayer *musicPlayer;
    void handleMediaError(QMediaPlayer::Error error);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
signals:
    void returnToMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void update();
    void restartGame();

private:
    void endGame(bool success);
    Player *player;
    QTimer *gameTimer;
    QMediaPlayer *mediaPlayer;
    QPushButton *retryButton;
    QPushButton *exitButton;
    NeonCityTrack *currentTrack; // Используем NeonCityTrack напрямую
    int obstacleSpawnTimer = 0;
    int gameTime = 0; // Track time in frames
};

#endif // GAMESCENE_H
