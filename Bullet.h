#ifndef SPACEINVADERS_BULLET_H
#define SPACEINVADERS_BULLET_H

#include <raylib.h>
#include <list>

class Bullet {
private:
    Rectangle m_rectangle;

public:
    explicit Bullet(Vector2 position);
    [[nodiscard]] Rectangle getRectangle() const;
    void draw(float speed);
    bool operator==(const Bullet& other) const;
    static void shoot(std::list<Bullet>* bullets, float x, float y, float shuttleSize, Sound shootSound, double* lastShotTime, bool audio);
};

#endif //SPACEINVADERS_BULLET_H