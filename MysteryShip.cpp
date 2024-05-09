#include "MysteryShip.h"

MysteryShip::~MysteryShip() {
    UnloadTexture(m_texture);
}

void MysteryShip::spawn() {
    if((GetTime() - m_timeLastSpawn > m_spawnInterval) && !m_alive) {
        m_timeLastSpawn = GetTime();
        m_spawnInterval = GetRandomValue(25, 40);

        m_rectangle.y = 100;

        if (GetRandomValue(0, 1)) {
            m_rectangle.x = 0;
            m_speed = 3;
        } else {
            m_rectangle.x = static_cast<float>(GetScreenWidth());
            m_speed = -3;
        }
        m_alive = true;
    }
}

void MysteryShip::Draw() {
    if (m_rectangle.x >= 0 && m_rectangle.x <= static_cast<float>(GetScreenWidth()) && m_alive) {
        m_rectangle.x += m_speed;
        DrawTexture(m_texture, static_cast<int>(m_rectangle.x), static_cast<int>(m_rectangle.y), WHITE);
    } else {
        m_alive = false;
    }
}

void MysteryShip::setAlive(bool alive) {
    m_alive = alive;
}

Rectangle MysteryShip::getRectangle() const {
    if (m_alive)
        return m_rectangle;
    else
        return {0, 0, 0, 0};
}

void MysteryShip::resetTimeLastSpawn() {
    m_timeLastSpawn = GetTime();
}