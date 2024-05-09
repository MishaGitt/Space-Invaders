#include "Block.h"

Block::Block(Rectangle rectangle) : m_rectangle(rectangle) {}

void Block::Draw() const {
    DrawRectangleRec(m_rectangle, YELLOW);
}

Rectangle Block::getRect() const {return m_rectangle; }