#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include "Shuttle.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "MysteryShip.h"

class Game{
public:
    std::list<Obstacle> obstacles;
    std::list<Block> blocks;
    std::list<Enemy> enemies;
    std::list<Bullet> bullets;
    std::list<Bullet> enemyBullets;

    Texture2D lifeTexture = LoadTexture("VFX/shuttle.png");
    Texture2D buttonUnmuted = LoadTexture("VFX/buttonUnmuted.png");
    Texture2D buttonMuted = LoadTexture("VFX/buttonMuted.png");

    Vector2 mousePoint = {0, 0};

    float enemySpeed = 1;
    double timeLastEnemyFired = 1.0;
    double lastShotTime{};

    unsigned int score = 0;
    unsigned int highscore = loadHighscoreFromFile();

    uint_fast8_t lives = 0;
    uint_fast8_t level = 0;

    int btnState = 1;
    Rectangle btnBounds{};

    MysteryShip mysteryShip;
    Shuttle shuttle;

    Shader redShader = LoadShader(nullptr, "resources/shaders/red_screen.fs");
    float redIntensity = 0.0;
    Shader greenShader = LoadShader(nullptr, "resources/shaders/green_screen.fs");
    float greenIntensity = 0.0;

    bool running = false;
    bool audio = true;

    Music music{};
    Sound shootSound{};
    Sound damageSound{};
    Sound mysteryShipHitSound{};

    Game();
    ~Game();
    void audioInit();
    void drawBtn() const;
    void drawAmmo() const;
    void drawShaders() const;
    void drawBackground() const;
    static void drawGameBorders();
    void drawScore() const;
    void drawPlayerLives() const;
    void reset();
    void update();
    void checkForCollisions();
    void checkPlayerBulletCollisions();
    void checkEnemyBulletCollisions();
    void checkForHighscore();
    void saveHighscoreToFile() const;
    static unsigned int loadHighscoreFromFile();
};

#endif //SPACEINVADERS_GAME_H
