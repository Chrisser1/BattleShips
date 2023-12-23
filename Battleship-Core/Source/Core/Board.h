#pragma once

#include <vector>
#include <iostream>
#include <iomanip> // Include for setw
#include "Utils.h"

class Board {
private:
    std::vector<std::vector<Utils::CellState>> grid;
    int boardSize;
    std::vector<Utils::ShipInfo> ships;

public:
    Board(int size); // Constructor to initialize the board with a given size

    // Function to place a ship on the board
    bool PlaceShip(Utils::ShipInfo ship, bool isAI);

    // Function to check if a coordinate is valid on the board
    bool IsValidCoordinate(Utils::Point point) const;

    // Function to check if a ship occupies a point
    bool IsOccupied(Utils::Point point) const;

    // Function to check if a placed ship would collide with another ship
    bool CollidesWithShip(Utils::Point start, Utils::Point end) const;

    // Function to perform an attack on the board
    void AttackCoordinate(Utils::Point point);

    // Funtion to check if a point contains a sunk ship
    bool HasSunkShip(Utils::Point pos);

    // Function to get a ship at a position
    Utils::ShipInfo* GetShipAtPosition(Utils::Point pos);

    // Function to check if all ships are sunk on the board
    bool AllShipsSunk() const;

    // Function to get occupied positions
    std::vector<Utils::Point> GetOccupiedPositions(Utils::ShipInfo ship) const;

    // Function to display the board (optional)
    void DisplayBoard(bool showShips) const;

    // Function to check if a position has been attacked
    bool HasAlreadyAttacked(Utils::Point pos);

    // Getter function to get the size of the board
    int GetBoardSize() { return boardSize; }

    // Getter to get cell state
    Utils::CellState GetCellState(Utils::Point pos) { return grid[pos.x][pos.y]; }

    // Getter to get ships
    std::vector<Utils::ShipInfo> GetShips() { return ships; }
};
