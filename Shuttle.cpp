#include "Shuttle.h"

Shuttle::~Shuttle() {
    UnloadTexture(m_texture);
}

Rectangle Shuttle::getRectangle() const {
    return m_rectangle;
}

void Shuttle::draw() const {
    DrawTexture(m_texture, static_cast<int>(m_rectangle.x), static_cast<int>(m_rectangle.y), WHITE);
}

void Shuttle::controls() {
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && m_rectangle.x > 10)
        m_rectangle.x -= m_speed;
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && m_rectangle.x + static_cast<float>(m_texture.width) < GetScreenWidth() - 10.0)
        m_rectangle.x += m_speed;
}