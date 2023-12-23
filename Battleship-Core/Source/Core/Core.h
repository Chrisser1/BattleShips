#pragma once

#include "Core/core.h" // Include necessary Battleship headers
#include "Player.h"
#include "Utils.h"
#include "BattleshipAI.h"
#include <iostream>
#include <string>

namespace Core {
    class BattleshipApp {
    public:
        BattleshipApp() : gameIsOver(false), player1(chooseBoardSize()), player2(player1.GetBoardSize()) {
            // Initialize player 1
            std::string name;
            std::cout << "Current player: Player1" << std::endl;
            std::cout << "Enter player name: ";
            std::getline(std::cin >> std::ws, name);
            player1.SetPlayerName(name);
            player1.PlaceShips();

            // Check if the opponent is an AI
            if (isOpponentAi()) {
                player2IsAI = true;
                opponentAI = new BattleshipAI(chooseAIType());
                opponentAI->PlaceShips(player2);
                player2.SetPlayerName("AI");
            }
            else {
                // Initialize player 2
                player2IsAI = false;
                std::cout << "Current player: Player2" << std::endl;
                std::cout << "Enter player name: ";
                std::getline(std::cin >> std::ws, name);
                player2.SetPlayerName(name);
                player2.PlaceShips();
            }
        }

        void Run();

    private:
        Player player1;
        Player player2;
        bool gameIsOver;
        BattleshipAI* opponentAI; // Pointer to AI instance
        bool player2IsAI;

    private:
        // Function to choose if you play against AI or not
        bool isOpponentAi();

        // Function to choose board size
        int chooseBoardSize();

        // Function to choose AI type
        int chooseAIType();

        // Function to display the person who wins
        void DisplayVictoryScreen(const std::string& winningPlayer);
    };
}

