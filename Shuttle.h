#ifndef SPACEINVADERS_SHUTTLE_H
#define SPACEINVADERS_SHUTTLE_H

#include <raylib.h>

class Shuttle {
private:
    Texture2D m_texture = LoadTexture("VFX/shuttle.png");

    Rectangle m_rectangle = {static_cast<float>((GetScreenWidth() - m_texture.width) / 2.0),
                             static_cast<float>(GetScreenHeight() - m_texture.height - 100),
                             static_cast<float>(m_texture.width),
                             static_cast<float>(m_texture.height)};
    float m_speed = 10;

public:
    ~Shuttle();
    [[nodiscard]] Rectangle getRectangle() const;
    void draw() const;
    void controls();
};

#endif //SPACEINVADERS_SHUTTLE_H