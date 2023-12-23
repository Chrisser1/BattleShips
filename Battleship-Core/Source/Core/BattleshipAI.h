#pragma once

#include "Player.h"
#include <cstdlib> // Include for rand()
#include <iomanip> // Include for setw

class BattleshipAI {
public:
    BattleshipAI(int type);

    // Function for the ai to attack
    void MakeAttack(Player& opponent);

    // Funktion to add adjacent squares
    void AddAdjacentSquares(const Utils::Point& point, Player& opponent);

    // Function to generate heat maps
    std::vector<std::vector<int>> GenerateProbabilityMap(Player& opponent);

    // Function to calculate endpoints
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> CalculateEndpoints(int row, int col, int useSize, int boardSize);

    // Function to update probabilities base on hits and none hits
    void UpdateProbabilities(std::vector<std::vector<int>>& probMap, int row, int col, Player& opponent);

    // Functiom to check if the ship can be placed
    bool CanPlaceShip(Board& board, const Utils::Point& start, const Utils::Point& end);

    // Function to update the probabilitymap
    void UpdateProbabilityMap(std::vector<std::vector<int>>& probMap, int startRow, int startCol, int endRow, int endCol);

    // Function to display the heat map
    void DisplayHeatmap(const std::vector<std::vector<int>>& heatmap);

    // Function to find the point with the highest probability
    Utils::Point FindHighestProbability(const std::vector<std::vector<int>>& probabilityMap, int boardSize) const;

    // Funtion to randomly place ships
    void PlaceShips(Player& player);

private:
    bool isHunting = true;
    bool isProbalistic;
    std::vector<Utils::Point> targetSquares;
};
