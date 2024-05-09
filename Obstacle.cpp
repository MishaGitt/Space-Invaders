#include "Obstacle.h"

std::vector<std::vector<uint_fast8_t>> Obstacle::grid = {
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}
};

Obstacle::Obstacle(Vector2 position) : m_position(position) {}

void Obstacle::draw(const std::list<Obstacle>& obstacles, const std::list<Block>& blocks) {
    for (auto obstacle : obstacles) {
        for (auto block: blocks) {
            block.Draw();
        }
    }
}

void Obstacle::initializeObstacles(std::list<Obstacle>& obstacles, float shuttlePosition, std::list<Block>& blocks) {

    blocks.clear();
    obstacles.clear();

    for (int i = 0; i < 4; i++) {
        obstacles.push_back(Obstacle({static_cast<float>(100 + i * 200), shuttlePosition}));
    }

    for (auto& obstacle : obstacles) {
        for (int row = 0; row < Obstacle::grid.size(); row++) {
            for (int column = 0; column < Obstacle::grid[row].size(); column++) {
                if (Obstacle::grid[row][column]) {
                    auto block = Block({obstacle.m_position.x + static_cast<float>(column) * obstacle.m_size,
                                        obstacle.m_position.y + static_cast<float>(row) * obstacle.m_size, obstacle.m_size, obstacle.m_size});
                    blocks.push_back(block);
                }
            }
        }
    }
}