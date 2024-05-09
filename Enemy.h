#ifndef SPACEINVADERS_ENEMY_H
#define SPACEINVADERS_ENEMY_H

#include <raylib.h>
#include <list>
#include <cstdint>

#include "Bullet.h"

class Enemy {
private:
    Vector2 m_position{};
    Texture2D m_texture{};

public:
    Enemy(Vector2 position, uint_fast8_t type);
    ~Enemy();
    [[nodiscard]] Rectangle getRectangle() const;
    void draw() const;
    static void updateEnemies(std::list<Enemy> &enemies, bool* running, float* speed, uint_fast8_t* lives);
    void moveDown(float speed);
    static void shoot(std::list<Enemy>& enemies, std::list<Bullet>& bullets, double* timeLastEnemyFired, uint_fast8_t level);
    static void initializeEnemies(std::list<Enemy>& enemies);
};

#endif //SPACEINVADERS_ENEMY_H