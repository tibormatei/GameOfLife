#pragma once

#include "Field.h"


class GameOfLife
{
    public:
        GameOfLife(size_t xAreaSize, size_t yAreaSize, size_t xViewSize, size_t yViewSize);
        ~GameOfLife();

    public:
        void startTheGame(unsigned int ages);

    private:
        bool initializeArea();
        void markDieCells() noexcept;
        void show() const noexcept;

    private:
        const size_t m_xAreaSize;
        const size_t m_yAreaSize;
        Field** area;

        size_t m_xViewSize;
        size_t m_yViewSize;
        size_t m_xViewStartIndex;
        size_t m_yViewStartIndex;


        bool m_isInitialized;
};
