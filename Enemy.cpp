#include "Enemy.h"

Enemy::Enemy(Vector2 position, uint_fast8_t type) : m_position(position) {
    switch (type) {
        case 0:
            m_texture = LoadTexture("VFX/alien_1.png");
            break;
        case 1:
            m_texture = LoadTexture("VFX/alien_2.png");
            break;
        case 2:
            m_texture = LoadTexture("VFX/alien_3.png");
            break;
        default:
            break;
    }
}

Enemy::~Enemy() = default;

Rectangle Enemy::getRectangle() const {
    return { m_position.x, m_position.y, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height)};
}

void Enemy::draw() const {
    DrawTexture(m_texture, static_cast<int>(m_position.x), static_cast<int>(m_position.y), WHITE);
}

void Enemy::moveDown(float speed) {
    m_position.y += speed;
}

void Enemy::updateEnemies(std::list<Enemy> &enemies, bool* running, float* speed, uint_fast8_t* lives) {

    for (auto &enemy : enemies) {
        if (enemy.m_position.x + static_cast<float>(enemy.m_texture.width) >= GetScreenWidth() - 10.0) {
            *speed = -1;
            for (auto &enemyToMoveDown : enemies)
                enemyToMoveDown.moveDown(10);
        } else if (enemy.m_position.x <= 10) {
            *speed = 1;
            for (auto &enemyToMoveDown : enemies)
                enemyToMoveDown.moveDown(10);
        } else if ((enemy.m_position.y + static_cast<float>(enemy.m_texture.height) >= GetScreenHeight() - 250.0)) {
            *running = false;
            *lives = 0;
        }

        enemy.m_position.x += *speed;
        enemy.draw();
    }

    if (enemies.empty()) {
        *running = false;
    }
}

void Enemy::shoot(std::list<Enemy>& enemies, std::list<Bullet>& bullets, double* timeLastEnemyFired, uint_fast8_t level) {

    float enemyLaserShootInterval;

    switch (level) {
        case 1:
            enemyLaserShootInterval = 0.3;
            break;
        case 2:
            enemyLaserShootInterval = 0.2;
            break;
        default:
            enemyLaserShootInterval = 0.1;
    }

    if (GetTime() - *timeLastEnemyFired >= enemyLaserShootInterval && !enemies.empty()) {
        int randomIndex = GetRandomValue(0, static_cast<int>(enemies.size()) - 1);
        auto enemy = enemies.begin();
        advance(enemy, randomIndex);
        bullets.emplace_back(enemy->m_position);
        *timeLastEnemyFired = GetTime();
    }

    for (auto &bullet : bullets) {
        bullet.draw(5);
    }
}

void Enemy::initializeEnemies(std::list<Enemy>& enemies) {
    for (int rows = 0; rows < 11; rows++) {
        for (int columns = 0; columns < 5; columns++) {
            enemies.push_front(Enemy({static_cast<float>(50 + 70 * rows), static_cast<float>(50 + 80 * columns)}, columns % 3));
        }
    }
}