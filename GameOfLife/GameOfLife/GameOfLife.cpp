#include "GameOfLife.h"
#include <new>
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;


GameOfLife::GameOfLife(size_t xAreaSize, size_t yAreaSize, size_t xViewSize, size_t yViewSize) :
    m_xAreaSize(xAreaSize), m_yAreaSize(yAreaSize), m_xViewSize(xViewSize), m_yViewSize(yViewSize), area(nullptr), m_isInitialized(true)
{
    try
    {
        area = new Field*[m_xAreaSize];

        register size_t i;
        for (i = 0; i < m_xAreaSize; ++i)
        {
            area[i] = nullptr;
        }
        for (i = 0; i < m_xAreaSize; ++i)
        {
            area[i] = new Field[m_yAreaSize];
        }
    }
    catch (const std::bad_alloc e)
    {
        m_isInitialized = false;
    }

    if (m_xViewSize > m_xAreaSize)
    {
        m_xViewSize = m_xAreaSize;
    }
    if (m_yViewSize > m_yAreaSize)
    {
        m_yViewSize = m_yAreaSize;
    }

    m_xViewStartIndex = ((m_xAreaSize - m_xViewSize) / 2);
    m_yViewStartIndex = ((m_yAreaSize - m_yViewSize) / 2);

    m_xViewSize += m_xViewStartIndex;
    m_yViewSize += m_yViewStartIndex;
}

GameOfLife::~GameOfLife()
{
    if (area != nullptr)
    {
        for (size_t i = 0; i < m_xAreaSize; ++i)
        {
            if (area[i] != nullptr)
            {
                delete[] area[i];
            }
        }

        delete[] area;
    }
}

void GameOfLife::startTheGame(unsigned int ages = 1)
{
    initializeArea();

    unsigned int age = ages;
    while (age)
    {
        markDieCells();
        show();
        --age;
    }
}

bool GameOfLife::initializeArea()
{
    std::srand(std::time(0));
    for (size_t i = m_xViewStartIndex; i < m_xViewSize; ++i)
    {
        for (size_t j = m_yViewStartIndex; j < m_yViewSize; ++j)
        {
            int r = std::rand() % 10;
            if (r < 3)
            {
                area[i][j].cell_exist = true;
            }
        }
    }

    return true;
}

void GameOfLife::markDieCells() noexcept
{

}

void GameOfLife::show() const noexcept
{
    if (m_isInitialized)
    {
        system("cls");

        for (size_t i = m_xViewStartIndex; i < m_xViewSize; ++i)
        {
            for (size_t j = m_yViewStartIndex; j < m_yViewSize; ++j)
            {
                if (area[i][j].cell_exist)
                {
                    cout << "#";
                }
                else
                {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }
}
