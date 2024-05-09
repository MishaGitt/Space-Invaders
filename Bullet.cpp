#include "Bullet.h"
#include <raymath.h>

Bullet::Bullet(Vector2 position) : m_rectangle({position.x, position.y, 3, 12}) {}

Rectangle Bullet::getRectangle() const {
    return m_rectangle;
}
void Bullet::draw(float speed) {
    m_rectangle.y += speed;
    DrawRectangleRec(m_rectangle, WHITE);
}

bool Bullet::operator==(const Bullet& other) const {
    return Vector2Equals({m_rectangle.x, m_rectangle.y}, {other.m_rectangle.x, other.m_rectangle.y});
}

void Bullet::shoot(std::list<Bullet>* bullets, float x, float y, float shuttleSize, Sound shootSound, double* lastShotTime, bool audio) {

    if (IsKeyDown(KEY_SPACE) && GetTime() - *lastShotTime >= 0.35 && bullets->size() < 3) {
        (*bullets).emplace_back(Bullet({x + shuttleSize/2, y}));
        *lastShotTime = GetTime();
        if (audio) PlaySound(shootSound);
    }

    for (auto &bullet : *bullets) {
        if (bullet.m_rectangle.y < 0)
            (*bullets).pop_front();
        bullet.draw(-8);
    }
}