#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include "MazeGenerator.h"

BOOL CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        // Handle these events gracefully, e.g., save data or perform cleanup
        std::cout << "Received a shutdown event. Performing cleanup..." << std::endl;
        // Perform necessary cleanup here
        Sleep(2000);  // Sleep to allow cleanup, replace with your cleanup logic
        return TRUE;
    default:
        return FALSE;
    }
}

int main() {
    int width = 0;
    int height = 0;
    

    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::cout << "Press Ctrl+C or close the console to test." << std::endl;

        while (width < 4 || height < 4)
        {
            std::cout << "Enter Maze width : ";
            std::cin >> width;

            std::cout << "Enter Maze height : ";
            std::cin >> height;
        }
        MazeGenerator maze(width, height);
        maze.generateMaze();
        std::cout << maze << std::endl;
        maze.solveMaze();
        maze.setShowSolution(false);
        std::cout << maze << std::endl;

        //maze.readFile("E:\\maze.txt");
        int index = std::rand() % 100000;
        maze.saveToFile("maze" + std::to_string(index) + ".txt");

        while (true)
        { 
            //waiting for user to close
        }
    }
    else {
        std::cerr << "SetConsoleCtrlHandler failed." << std::endl;
        return 1;
    }

    return 0;
}
