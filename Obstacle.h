#ifndef SPACEINVADERS_OBSTACLE_H
#define SPACEINVADERS_OBSTACLE_H

#include <raylib.h>
#include <vector>
#include <cstdint>

#include "Bullet.h"
#include "Block.h"

class Obstacle {
private:
    Vector2 m_position{};
    std::list<Block> blocks;
    float m_size = 7;
    static std::vector<std::vector<uint_fast8_t>> grid;

public:
    static void draw(const std::list<Obstacle>& obstacles, const std::list<Block>& blocks);
    explicit Obstacle(Vector2 position);
    static void initializeObstacles(std::list<Obstacle>& obstacles, float shuttlePosition, std::list<Block>& blocks);
};

#endif //SPACEINVADERS_OBSTACLE_H