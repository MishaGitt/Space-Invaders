#ifndef SPACEINVADERS_BLOCK_H
#define SPACEINVADERS_BLOCK_H

#include <raylib.h>

class Block {
private:
    Rectangle m_rectangle;

public:
    explicit Block(Rectangle rectangle);
    void Draw() const;
    [[nodiscard]] Rectangle getRect() const;
};


#endif //SPACEINVADERS_BLOCK_H
