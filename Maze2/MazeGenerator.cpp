#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(int height, int width)
{
    // Make width and height odd
    m_width = 2 * (width / 2) + 1;
    m_height = 2 * (height / 2) + 1;

    // Initialize the grid with 0 values
    m_grid = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_RndGen.seed(std::random_device()()); // get random seed for true randomization;

    // Declare instance variables
    m_path.clear();
    m_directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
    m_mazeIsGenerated = false;
    m_solution.clear();
    m_showSolutionMaze = false;
    m_start = {2, 0};// { m_height - 1, 0 };
    m_end = { m_height - 1, m_width - 1 }; //{ 0, m_width - 1 };

    // Fill unvisited cells
    for (int row = 0; row < m_height; row++) {
        for (int col = 0; col < m_width; col++) {
            if (row % 2 == 0 && col % 2 == 0) {
                m_unvisited.push_back({ row, col });
            }
        }
    }
}

MazeGenerator::~MazeGenerator()
{
}

void MazeGenerator::generateMaze()
{
    // Reset the grid before generation
    initializeGrid();

    int firstIndex = getRandomCell(0, m_unvisited.size() - 1);
    Cell currentCell = m_unvisited[firstIndex];
    markCellAsVisited(currentCell);

    // Loop until all cells have been visited
    while (!m_unvisited.empty()) {
        // Choose a random cell to start the walk
        int randomIndex = getRandomCell(0, m_unvisited.size() - 1);
        Cell startCell = m_unvisited[randomIndex];
        currentCell = startCell;

        performRandomWalkUntilVisitedCell(currentCell);

        currentCell = startCell;
        tracePathAndMarkVisited(currentCell);
    }

    m_mazeIsGenerated = true;
}

void MazeGenerator::setShowSolution(bool show)
{
    m_showSolutionMaze = show;
}

void MazeGenerator::saveToFile(std::string fileName)
{
    std::stringstream sstream;
    sstream << "1";
    for (int j = 0; j <= m_width; j++) {
        sstream << "1";
    }
    sstream << "\n";
    for (int i = 0; i < m_height; i++) {

        if (i == m_start.first) {
            sstream << "0"; // maze entrance cell
        }
        else {
            sstream << "1";
        }
        for (int j = 0; j < m_width; j++) {
            if (m_grid[i][j] == 0) {
                sstream << "1";
            }
            else {
                if (!m_showSolutionMaze) {
                    sstream << "0";
                }
                else {
                    Cell cell = { i, j };
                    if (std::find(m_solution.begin(), m_solution.end(), cell) != m_solution.end()) {
                        sstream << "2";
                    }
                    else {
                        sstream << "0";
                    }
                }
            }
        }
        if (i == m_end.first) {
            sstream << "0\n"; // maze exit cell
        }
        else {
            sstream << "1\n";
        }
    }

    sstream << "1";
    for (int j = 0; j <= m_width; j++) {
        sstream << "1";
    }

    auto exeDir = getExeDir();
    std::string executablePath = exeDir + "\\" + fileName;
    std::ofstream mazeFile(executablePath);
    mazeFile << sstream.rdbuf();
    mazeFile.close();

}

void MazeGenerator::readFile(std::string fileName)
{
    std::ifstream mazeFile(fileName);

    std::vector<std::vector<bool>> matrix;
    std::string line;

    // Read the file line by line
    while (std::getline(mazeFile, line)) {
        std::vector<bool> row;

        // Parse each character in the line
        for (char c : line) {
            if (c == '0') {
                row.push_back(false);
            }
            else if (c == '1') {
                row.push_back(true);
            }
            else {
                std::cerr << "Invalid character in the file." << std::endl;

            }
        }

        // Add the row to the matrix
        matrix.push_back(row);
    }
    // Print the 2D vector of boolean values

    std::cout << "Maze content : " << std::endl;
    for (const auto& row : matrix) {
        for (bool value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void MazeGenerator::initializeGrid()
{
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            m_grid[i][j] = 0;
        }
    }

    m_unvisited.clear();
    m_visited.clear();
    m_path.clear();
    m_mazeIsGenerated = false;

    for (int row = 0; row < m_height; row++) {
        for (int col = 0; col < m_width; col++) {
            if (row % 2 == 0 && col % 2 == 0) {
                m_unvisited.push_back({ row, col });
            }
        }
    }
}

void MazeGenerator::solveMaze()
{
    if (!m_mazeIsGenerated) {
        return;
    }

    m_path.clear();
    Cell current = m_start;
  
    while (current != m_end) {
        bool foundValidDirection = false;

        while (!foundValidDirection) {
            int dirNum = rand() % 4;
            Cell neighborCell = getNextCell(current, dirNum, 1);

            if (isValidDirection(current, dirNum) && m_grid[neighborCell.first][neighborCell.second] != 0) {
                m_path[current] = dirNum;
                current = getNextCell(current, dirNum, 2);
                foundValidDirection = true;
            }
        }
    }

    current = m_start;
    m_solution.push_back(current);

    while (current != m_end) {
        int dirNum = m_path[current];
        Cell crossed = getNextCell(current, dirNum, 1);
        current = getNextCell(current, dirNum, 2);
        m_solution.push_back(crossed);
        m_solution.push_back(current);
    }

    m_path.clear();
}

Cell MazeGenerator::getNextCell(const Cell& cell, int dirNum, int moveBy)
{
    Cell dirTup = m_directions[dirNum];
    return Cell{ cell.first + moveBy * dirTup.first, cell.second + moveBy * dirTup.second };
}

bool MazeGenerator::isValidDirection(const Cell& cell, int dirNum)
{
    // TODO : optimize!!!
    Cell newCell = getNextCell(cell, dirNum, 2);
    bool outOfMazeLeftSide = newCell.first < 0 || newCell.second < 0;
    bool outOfMazeRightSide = newCell.first >= m_height || newCell.second >= m_width;
    return !(outOfMazeLeftSide || outOfMazeRightSide);
}

void MazeGenerator::removeTheWall(const Cell& cell)
{
    m_grid[cell.first][cell.second] = 1;
}

int MazeGenerator::getRandomCell(int rangeMin, int rangeMax)
{
    std::uniform_int_distribution<int> dist(rangeMin, rangeMax);
    return dist(m_RndGen);
}

int MazeGenerator::getRandomValidDirection(const Cell& currentCell)
{
    int dirNum;
    do {
        dirNum = rand() % 4;
    } while (!isValidDirection(currentCell, dirNum));
    return dirNum;
}

bool MazeGenerator::isCellVisited(const Cell& cell)
{
    return std::find(m_visited.begin(), m_visited.end(), cell) != m_visited.end();
}


void MazeGenerator::performRandomWalkUntilVisitedCell(Cell& current)
{
    while (true) {
        int direction = rand() % 4;

        // Find a valid direction
        while (!isValidDirection(current, direction)) {
            direction = rand() % 4;
        }

        m_path[current] = direction;
        current = getNextCell(current, direction, 2);

        if (isCellVisited(current)) {
            break;
        }
    }
}

void MazeGenerator::tracePathAndMarkVisited(Cell& current)
{
    while(true) {
        markCellAsVisited(current);
        int direction = m_path[current];
        Cell neighborCell = getNextCell(current, direction, 1);
        removeTheWall(neighborCell);
        current = getNextCell(current, direction, 2);

        if (isCellVisited(current)) {
            m_path.clear();
            break;
        }
    }
}

void MazeGenerator::markCellAsVisited(Cell& cell)
{
    m_visited.push_back(cell);
    m_unvisited.erase(std::remove(m_unvisited.begin(), m_unvisited.end(), cell), m_unvisited.end());
    removeTheWall(cell);
}

std::vector<std::vector<int>> MazeGenerator::getGrid()
{
    return m_grid;
}

std::vector<Cell> MazeGenerator::getSolution()
{
    return m_solution;
}

std::string MazeGenerator::getExeDir()
{
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);

    // Extract the directory path from the full path
    std::wstring exePath(buffer);
    std::wstring exeDir = exePath.substr(0, exePath.find_last_of(L"\\/"));
    std::string narrowExeDir(exeDir.begin(), exeDir.end());
    return narrowExeDir;
}

std::ostream& operator<<(std::ostream& os, const MazeGenerator& maze)
{
    std::string wallSymbol = "1";
    std::string outsideWallSymbol = "|";
    std::string spaceSymbol = " ";
    os << wallSymbol.c_str();
    for (int j = 0; j <= maze.m_width; j++) {
        os << wallSymbol.c_str();
    }
    os << "\n";

    for (int i = 0; i < maze.m_height; i++) {
        
        if (i == maze.m_start.first) {
            os << spaceSymbol.c_str(); // maze entrance cell
        }
        else {
            os << outsideWallSymbol.c_str();
        }
        for (int j = 0; j < maze.m_width; j++) {
            if (maze.m_grid[i][j] == 0) {
                os << wallSymbol.c_str();
            }
            else {
                if (!maze.m_showSolutionMaze) {
                    os << spaceSymbol.c_str();
                }
                else {
                    Cell cell = { i, j };
                    if (std::find(maze.m_solution.begin(), maze.m_solution.end(), cell) != maze.m_solution.end()) {
                        os << "*";
                    }
                    else {
                        os << spaceSymbol.c_str();
                    }
                }
            }
        }
        if (i == maze.m_end.first) {
            os << spaceSymbol.c_str() << "\n"; // maze exit cell
        }
        else {
            os << outsideWallSymbol.c_str() <<"\n";
        }
    }

    os << wallSymbol.c_str();
    for (int j = 0; j <= maze.m_width; j++) {
        os << wallSymbol.c_str();
    }
    return os;
}
