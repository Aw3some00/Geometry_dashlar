#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QTimer>
#include "player.h"
#include <QSoundEffect>
#include <QAudioOutput>
#include <QPushButton>
#include "NeonCityTrack.h"
#include <QGraphicsSceneMouseEvent>
int getRandomInt(int min, int max);
static const QStringList soundFiles = {
    "qrc:/fail.mp3",
    "qrc:/ahhhh-gachi-muchi.mp3",
    "qrc:/brazil-zil.mp3",
    "qrc:/cat-laugh-meme-1.mp3",
    "qrc:/demir-boru-dusme-sesi.mp3",
    "qrc:/directed-by-robert-b_voI2Z4T.mp3",
    "qrc:/gay-echo.mp3",
    "qrc:/goida_hRZ6vDr.mp3",
    "qrc:/gta-v-wastedbusted-gaming-sound-effect-hd.mp3",
    "qrc:/movie_1.mp3",
    "qrc:/shocked-sound-effect.mp3",
    "qrc:/spongebob-fail.mp3",
    "qrc:/troll-face-smiling.mp3",
    "qrc:/y2mate_xi2QMCt.mp3",
    "qrc:/selamun-aleykum_KlyTyfa.mp3"
};
static const QStringList backsounds = {
    "qrc:/kutay_ucar_mvsn.mp3",
    "qrc:/nokia-ringtone-arabic.mp3",
    "qrc:/fon.mp3"

};
class GameScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();
    void fonstop();
    QMediaPlayer *fon=nullptr;

    void startGame();
    void setTrack(int trackId);
    bool isOnPlatform = false;
    bool isGameOver = false;
    void failstop();
    void stopMusic();
    QAudioOutput   audioOutput;
    void startMusic();
    QMediaPlayer *faultSound =nullptr;
    void fail();
    QMediaPlayer*win=nullptr;
    QSoundEffect* musicEffect;
    QMediaPlayer *musicPlayer = nullptr;
    void mute();
signals:
    void returnToMenu();
    void menuRequested();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void update();
    void restartGame();

private:
 void restartPlayback();
    Player *player;
    QTimer *gameTimer;
    QPushButton *retryButton;
    QPushButton *exitButton;
    NeonCityTrack *currentTrack; // Используем NeonCityTrack напрямую
    int obstacleSpawnTimer = 0;
    int gameTime = 0; // Track time in frames
};

#endif // GAMESCENE_H
