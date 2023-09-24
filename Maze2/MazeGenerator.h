#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <windows.h>

typedef std::pair<int, int> Cell;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};


class MazeGenerator
{
public:
    MazeGenerator(int height, int width);
    ~MazeGenerator();
    void generateMaze();
    void solveMaze();
    void setShowSolution(bool show);
    friend std::ostream& operator<<(std::ostream& os, const MazeGenerator& maze);
    void saveToFile(std::string fileName);
    void readFile(std::string fileName);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<int>> m_grid;
    std::vector<Cell> m_visited;
    std::vector<Cell> m_unvisited;
    std::unordered_map<Cell, int, pair_hash > m_path;
    std::vector<Cell> m_directions;
    std::vector<Cell> m_solution;

    bool m_mazeIsGenerated;
    bool m_showSolutionMaze;
    Cell m_start; // need only for maze solvation.
    Cell m_end;   // need only for maze solvation.
    std::mt19937 m_RndGen;

    void initializeGrid();
    Cell getNextCell(const Cell& cell, int dirNum, int moveBy);
    bool isValidDirection(const Cell& cell, int dirNum);
    void removeTheWall(const Cell& cell);
    int getRandomCell(int rangeMin, int rangeMax);
    int getRandomValidDirection(const Cell& currentCell);
    bool isCellVisited(const Cell& cell);
    void performRandomWalkUntilVisitedCell(Cell& current);
    void tracePathAndMarkVisited(Cell& current);
    void markCellAsVisited(Cell& cell);

    std::vector<std::vector<int>> getGrid(); // hashed grid
    std::vector<Cell> getSolution();

    std::string getExeDir();

};

