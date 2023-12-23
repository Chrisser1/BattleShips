#pragma once

#include "Board.h"
#include "Utils.h"
#include <iostream>
#include <string>

class Player {
private:
    Board playerBoard; // Player's game board
    std::string playerName;

public:
    // Constructor to initialize the player's board
    Player(int boardSize);

    // Function for the player to make an attack
    void MakeAttack(Player& opponent);

    // Function to check if all ships are sunk
    bool AllShipsSunk();

    // Function for the player to place a ship
    bool PlaceShip(Utils::BoatType shipType, Utils::Point pos, bool isVertical);

    // Function for the player to place all of their ships
    void PlaceShips();

    // Function to display the player's board
    void DisplayPlayerBoard() const;

    // Function to set playerName
    void SetPlayerName(std::string name);

    // Getter to return the name of the player
    std::string GetPlayerName() { return playerName; }

    // Getter to return player's board size
    int GetBoardSize() { return playerBoard.GetBoardSize(); }

    // Getter to return board
    Board& getBoard() { return playerBoard; }
private:
    // Helper function to get valid position input from the player
    bool GetValidPositionInput(Utils::Point& pos);

    // Helper function to get vertical placement input from the player
    bool GetVerticalPlacementInput(bool& isVertical);

    // Helper function to place a single ship
    void PlaceSingleShip(const std::pair<Utils::BoatType, std::pair<int, std::string>>& boat);
};
