#ifndef SPACEINVADERS_MYSTERYSHIP_H
#define SPACEINVADERS_MYSTERYSHIP_H

#include <raylib.h>

class MysteryShip {
private:
    double m_timeLastSpawn = GetTime();
    double m_spawnInterval = GetRandomValue(25, 40);
    Texture2D m_texture = LoadTexture("VFX/mystery.png");
    Rectangle m_rectangle = {0, 100, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height)};
    float m_speed{};
    bool m_alive = false;
public:
    ~MysteryShip();
    void resetTimeLastSpawn();
    void setAlive(bool alive);
    void spawn();
    void Draw();
    [[nodiscard]] Rectangle getRectangle() const;
};

#endif //SPACEINVADERS_MYSTERYSHIP_H