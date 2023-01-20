#pragma once

#include <memory>
#include "Field.h"


class GameOfLife
{
    public:
        GameOfLife(unsigned int xAreaSize, unsigned int yAreaSize, unsigned int xViewSize, unsigned int yViewSize);
        ~GameOfLife();

    private:
        const unsigned int m_xAreaSize;
        const unsigned int m_yAreaSize;
        unsigned int m_xViewSize;
        unsigned int m_yViewSize;
        Field **area;
};
