#include "GameOfLife.h"
#include <windows.h>
#include <new>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::getline;


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

    m_xViewStartIndex = (m_xAreaSize - m_xViewSize) / 2;
    m_yViewStartIndex = (m_yAreaSize - m_yViewSize) / 2;

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

void GameOfLife::startTheGame(unsigned int ages, bool fromFile)
{
    if (m_isInitialized)
    {
        bool initedArea = false;
        if (fromFile)
        {
            initedArea = initializeAreaFromFile();
            if (initedArea == false)
            {
                initedArea = initializeAreaRandomly();
            }
        }
        else
        {
            initedArea = initializeAreaRandomly();
        }

        if (initedArea)
        {
            unsigned int age = ages;
            while (age)
            {
                show();
                markDieCells();
                birthNewCells();
                removeDeadCells();
                --age;
            }
        }
    }
}

bool GameOfLife::initializeAreaRandomly()
{
    std::srand(std::time(0));
    for (size_t i = 0; i < m_xAreaSize; ++i)
    {
        for (size_t j = 0; j < m_yAreaSize; ++j)
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

bool GameOfLife::initializeAreaFromFile()
{
    bool succes = false;
    std::ifstream file("map.txt");

    if (file.is_open())
    {
        std::vector<std::string> lines;

        std::string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();

        size_t x_startIndex = (m_xAreaSize - lines.size()) / 2;
        size_t y_startIndex = (m_yAreaSize - lines[0].size()) / 2;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            for (size_t j = 0; j < lines[i].size(); ++j)
            {
                if (lines[i].at(j) == 'x')
                {
                    area[x_startIndex + i][y_startIndex + j].cell_exist = true;
                }
            }
        }

        succes = true;
    }

    return succes;
}

void GameOfLife::markDieCells() noexcept
{
    register unsigned char neighbor_counter = 0;

    for (size_t i = 0; i < m_xAreaSize; ++i)
    {
        for (size_t j = 0; j < m_yAreaSize; ++j)
        {
            if (area[i][j].cell_exist)
            {
                neighbor_counter = neighborCounter(i, j);

                if ((neighbor_counter < 2) || (neighbor_counter > 3))
                {
                    area[i][j].cell_will_die = true;
                }
                neighbor_counter = 0;
            }
        }
    }
}

void GameOfLife::birthNewCells() noexcept
{
    register unsigned char neighbor_counter = 0;

    for (size_t i = 0; i < m_xAreaSize; ++i)
    {
        for (size_t j = 0; j < m_yAreaSize; ++j)
        {
            if (area[i][j].cell_exist == false)
            {
                neighbor_counter = neighborCounter(i, j);

                if (neighbor_counter == 3)
                {
                    area[i][j].cell_will_birth = true;
                }
                neighbor_counter = 0;
            }
        }
    }

    for (size_t i = 0; i < m_xAreaSize; ++i)
    {
        for (size_t j = 0; j < m_yAreaSize; ++j)
        {
            if (area[i][j].cell_will_birth == true)
            {
                area[i][j].cell_exist = true;
                area[i][j].cell_will_birth = false;
            }
        }
    }
}

void GameOfLife::removeDeadCells() noexcept
{
    for (size_t i = 0; i < m_xAreaSize; ++i)
    {
        for (size_t j = 0; j < m_yAreaSize; ++j)
        {
            if (area[i][j].cell_will_die)
            {
                area[i][j].cell_exist = false;
                area[i][j].cell_will_die = false;
            }
        }
    }
}

unsigned char GameOfLife::neighborCounter(size_t& i, size_t& j) noexcept
{
    unsigned char neighbor_counter = 0;

    bool is_up_border    = (i > 0)               ? false : true;
    bool is_right_border = (j < m_yAreaSize - 1) ? false : true;
    bool is_down_border  = (i < m_xAreaSize - 1) ? false : true;
    bool is_left_border  = (j > 0)               ? false : true;

    // up
    if (!is_up_border)
    {
        if (area[i - 1][j].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // up & right
    if (!is_up_border && !is_right_border)
    {
        if (area[i - 1][j + 1].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // right
    if (!is_right_border)
    {
        if (area[i][j + 1].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // right & down
    if (!is_right_border && !is_down_border)
    {
        if (area[i + 1][j + 1].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // down
    if (!is_down_border)
    {
        if (area[i + 1][j].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // down & left
    if (!is_down_border && !is_left_border)
    {
        if (area[i + 1][j - 1].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // left
    if (!is_left_border)
    {
        if (area[i][j - 1].cell_exist)
        {
            neighbor_counter++;
        }
    }
    // left & up
    if (!is_left_border && !is_up_border)
    {
        if (area[i - 1][j - 1].cell_exist)
        {
            neighbor_counter++;
        }
    }

    return neighbor_counter;
}

void GameOfLife::show() const noexcept
{
    // system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, {0, 0});

    for (size_t i = m_xViewStartIndex; i < m_xViewSize; ++i)
    {
        for (size_t j = m_yViewStartIndex; j < m_yViewSize; ++j)
        {
            if (area[i][j].cell_exist)
            {
                cout << (char)178;
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }
}
