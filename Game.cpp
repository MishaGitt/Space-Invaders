#include "Game.h"
#include <string>
#include <fstream>

static std::string FormatWithLeadingZeros(unsigned int number, int width) {
    std::string numberText = std::to_string(number);
    unsigned int leadingZeros = width - numberText.length();
    return std::string(leadingZeros, '0') + numberText;
}

Game::Game() {
    Image windowIcon = LoadImage("VFX/logo.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);

    SetShaderValue(redShader, GetShaderLocation(redShader, "redIntensity"), &redIntensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(greenShader, GetShaderLocation(greenShader, "greenIntensity"), &greenIntensity, SHADER_UNIFORM_FLOAT);

    audioInit();

    btnBounds = {15, static_cast<float>(GetScreenHeight() - 85), static_cast<float>(buttonUnmuted.width), static_cast<float>(buttonUnmuted.height)};

    SetTargetFPS(60);
}

Game::~Game() {
    saveHighscoreToFile();

    UnloadTexture(lifeTexture);
    UnloadTexture(buttonUnmuted);
    UnloadTexture(buttonMuted);

    StopMusicStream(music);
    CloseAudioDevice();

    UnloadShader(greenShader);
    UnloadShader(redShader);

    bullets.clear();
    enemyBullets.clear();
    blocks.clear();
    obstacles.clear();
    enemies.clear();

    mysteryShip.~MysteryShip();
    shuttle.~Shuttle();
}

void Game::reset() {
    level++;
    bullets.clear();
    enemyBullets.clear();
    mysteryShip.resetTimeLastSpawn();

    if (!lives || !level) {
        level = 1;
        lives = 3;
        score = 0;

        enemies.clear();
        Obstacle::initializeObstacles(obstacles, shuttle.getRectangle().y-100, blocks);
    }

    if (!(level % 3)) {
        Obstacle::initializeObstacles(obstacles, shuttle.getRectangle().y - 100, blocks);
    }

    Enemy::initializeEnemies(enemies);
    running = true;
}

void Game::audioInit() {
    InitAudioDevice();
    music = LoadMusicStream("SFX/music.ogg");
    SetMusicVolume(music, 0.5);
    PlayMusicStream(music);

    shootSound = LoadSound("SFX/shoot.ogg");
    damageSound = LoadSound("SFX/damage.ogg");
    mysteryShipHitSound = LoadSound("SFX/mysteryShip.mp3");
}

void Game::drawBackground() const {
    BeginDrawing();
    ClearBackground({28, 31, 32, 255});

    drawGameBorders();
    drawPlayerLives();
    drawScore();
    drawShaders();
    drawBtn();

    EndDrawing();
}

void Game::drawBtn() const {
    if (btnState == 1)
        DrawTexture(buttonUnmuted, 15, GetScreenHeight() - 85, WHITE);
    else
        DrawTexture(buttonMuted, 15, GetScreenHeight() - 85, WHITE);
}

void Game::drawShaders() const {
    BeginShaderMode(redShader);
    DrawRectangle(0, 0, 950, 950, Fade(RED, redIntensity));
    EndShaderMode();

    BeginShaderMode(greenShader);
    DrawRectangle(0, 0, 950, 950, Fade(GREEN, greenIntensity));
    EndShaderMode();
}

void Game::drawAmmo() const {
    for (int i = 0; i < 3 - bullets.size(); i++) {
        DrawRectangle(static_cast<int>(shuttle.getRectangle().x + static_cast<float>(i) * 32),
                      static_cast<int>(shuttle.getRectangle().y + 20),
                      7, 25, WHITE);
    }
}

void Game::drawGameBorders() {
    DrawRectangleRoundedLines({5, 5, static_cast<float>(GetScreenWidth() - 10), static_cast<float>(GetScreenHeight() - 10)}, 0.1, 1, 2, YELLOW);
    DrawLine(5, GetScreenHeight() - 100, GetScreenWidth() - 5, GetScreenHeight()-100, YELLOW);
}

void Game::drawPlayerLives() const {
    for (int life = 0; life < lives; life++) {
        DrawTexture(lifeTexture, 100 + life * 80, GetScreenHeight() - 75, WHITE);
    }
}

void Game::drawScore() const {
    DrawText("SCORE", 50, 15, 34, YELLOW);
    DrawText(FormatWithLeadingZeros(score, 7).c_str(), 50 + MeasureText("SCORE", 34) * 5 / 4, 15, 34, YELLOW);

    DrawText("HIGHSCORE", GetScreenWidth() - MeasureText("HIGHSCORE", 30) - 50, 15, 30, YELLOW);
    DrawText(FormatWithLeadingZeros(highscore, 7).c_str(), GetScreenWidth() - MeasureText("HIGHSCORE", 30), 50, 30, YELLOW);

    if (lives) {
    DrawText("LEVEL", GetScreenWidth() - MeasureText("LEVEL", 40) - 100, GetScreenHeight() - 75, 40, YELLOW);
    DrawText(FormatWithLeadingZeros(level, 2).c_str(), GetScreenWidth() - MeasureText("LEVEL", 40) + 60, GetScreenHeight() - 75, 40, YELLOW);
    }
}

void Game::checkForCollisions() {
    checkPlayerBulletCollisions();
    checkEnemyBulletCollisions();
}

void Game::checkPlayerBulletCollisions() {

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bool bulletRemoved = false;

        for (auto enemy = enemies.begin(); enemy != enemies.end();) {
            if (CheckCollisionRecs(bullet->getRectangle(), enemy->getRectangle())) {
                enemy = enemies.erase(enemy);
                bulletRemoved = true;
                score += GetRandomValue(81, 114);
            } else {
                ++enemy;
            }
        }

        if (!bulletRemoved) {
            for (auto obstacle : obstacles) {
                for (auto block = blocks.begin(); block != blocks.end();) {
                    if (CheckCollisionRecs(bullet->getRectangle(), block->getRect())) {
                        block = blocks.erase(block);
                        bulletRemoved = true;
                    } else {
                        ++block;
                    }
                }
            }
        }

        if (!bulletRemoved && CheckCollisionRecs(bullet->getRectangle(), mysteryShip.getRectangle())) {
            bullet = bullets.erase(bullet);
            mysteryShip.setAlive(false);
            score += GetRandomValue(300, 350);

            if (audio) PlaySound(mysteryShipHitSound);
            greenIntensity = 0.6;

            if (lives < 3) {
                lives++;
            }
        }

        if (!bulletRemoved) {
            ++bullet;
        } else {
            bullet = bullets.erase(bullet);
        }
    }

    greenIntensity -= 0.01;
    if (greenIntensity < 0.0) greenIntensity = 0.0;

}

void Game::checkEnemyBulletCollisions() {
    for (auto bullet = enemyBullets.begin(); bullet != enemyBullets.end(); ) {
        for (auto obstacle : obstacles) {
            for (auto block = blocks.begin(); block != blocks.end(); ) {
                if (CheckCollisionRecs(bullet->getRectangle(), block->getRect())) {
                    block = blocks.erase(block);
                    bullet = enemyBullets.erase(bullet);
                } else {
                    ++block;
                }
            }
        }

        if (CheckCollisionRecs(bullet->getRectangle(), shuttle.getRectangle())) {
            if (--lives <= 0) {
                running = false;
            }

            if (audio) PlaySound(damageSound);
            if (lives) redIntensity = 0.6;

            bullet = enemyBullets.erase(bullet);
        } else if (bullet->getRectangle().y + bullet->getRectangle().height >= shuttle.getRectangle().y + float(shuttle.getRectangle().height)) {
            bullet = enemyBullets.erase(bullet);
        } else {
            ++bullet;
            redIntensity -= 0.01;
            if (redIntensity < 0.0) redIntensity = 0.0;
        }
    }
}

void Game::update() {
    while (!WindowShouldClose()) {

        mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, btnBounds) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            audio = !audio;
            btnState = -btnState;
        }

        if (audio) UpdateMusicStream(music);

        if (running) {

            mysteryShip.spawn();
            mysteryShip.Draw();
            shuttle.draw();
            drawAmmo();
            shuttle.controls();
            Obstacle::draw(obstacles, blocks);
            Bullet::shoot(&bullets, shuttle.getRectangle().x, shuttle.getRectangle().y, shuttle.getRectangle().width, shootSound, &lastShotTime, audio);
            Enemy::shoot(enemies, enemyBullets, &timeLastEnemyFired, level);
            Enemy::updateEnemies(enemies, &running, &enemySpeed, &lives);
            checkForCollisions();
            checkForHighscore();

        } else {

            DrawText("Press «Enter» to start", GetScreenWidth()/2 - MeasureText("Press «Enter» to start", 40)/2, GetScreenHeight()/2, 40, WHITE);

            if (lives && level) {
                DrawText("Level completed", GetScreenWidth()/2 - MeasureText("Level completed", 40)/2, GetScreenHeight()/2 - 40, 40, YELLOW);
            } else if (!lives && level) {
                DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 40)/2, GetScreenHeight()/2 - 40, 40, YELLOW);
                saveHighscoreToFile();
            }

            if (IsKeyDown(KEY_ENTER)) {
                reset();
            }
        }
        drawBackground();
    }
}

void Game::checkForHighscore() {
    if(score > highscore) {
        highscore = score;
    }
}

void Game::saveHighscoreToFile() const
{
    if (highscore > loadHighscoreFromFile()) {
        std::ofstream highscoreFile("highscore.txt");
        if (highscoreFile.is_open()) {
            highscoreFile << highscore;
            highscoreFile.close();
        }
    }
}

unsigned int Game::loadHighscoreFromFile() {
    unsigned int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");

    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    } else {
        loadedHighscore = 0;
    }

    return loadedHighscore;
}