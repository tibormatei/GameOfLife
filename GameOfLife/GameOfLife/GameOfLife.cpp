#include "GameOfLife.h"


GameOfLife::GameOfLife(unsigned int xAreaSize, unsigned int yAreaSize, unsigned int xViewSize, unsigned int yViewSize) :
    m_xAreaSize(xAreaSize), m_yAreaSize(yAreaSize), m_xViewSize(xViewSize), m_yViewSize(yViewSize), area(nullptr)
{
    area = new Field*[m_xAreaSize];
    for (unsigned int i = 0; i < m_xAreaSize; ++i)
    {
        area[i] = new Field[m_yAreaSize];
    }
}

GameOfLife::~GameOfLife()
{
    for (unsigned int i = 0; i < m_xAreaSize; ++i)
    {
        delete[] area[i];
    }
    delete[] area;
}
