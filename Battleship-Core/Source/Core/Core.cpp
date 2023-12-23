#include "Core.h"

namespace Core {
    void BattleshipApp::Run() {
        // Game loop
        Player* currentPlayer = &player1; // Start with Player 1
        Player* opponentPlayer = &player2;

        while (!gameIsOver) {
            // Display the game state
            opponentPlayer->DisplayPlayerBoard();

            // Allow the current player to make a move
            std::cout << "Current player's turn: " << currentPlayer->GetPlayerName() << std::endl;

            if (currentPlayer == &player2 && player2IsAI) {
                // AI player's turn
                opponentAI->MakeAttack(*opponentPlayer);
            }
            else {
                // Human player's turn or Player 1's turn
                currentPlayer->MakeAttack(*opponentPlayer);
            }

            // Check if the opponent's ships are all sunk
            if (opponentPlayer->AllShipsSunk()) {
                gameIsOver = true;
                DisplayVictoryScreen(currentPlayer->GetPlayerName());
            }

            // Swap players for the next turn
            std::swap(currentPlayer, opponentPlayer);
        }
    }

    bool BattleshipApp::isOpponentAi()
    {
        char choice;
        bool validChoice = false;

        do {
            std::cout << "Do you want to play against an AI? (Y/N): ";
            std::cin >> choice;

            // Convert input to uppercase for case-insensitive comparison
            choice = std::toupper(choice);

            if (choice != 'Y' && choice != 'N') {
                std::cout << "Please choose either Y or N." << std::endl;
            }
            else {
                validChoice = true;
            }

        } while (!validChoice);

        return (choice == 'Y');
    }

    int BattleshipApp::chooseBoardSize() {
        int size;
        std::cout << "Choose the board size (e.g., 10 for an 10x10 board): ";
        std::cin >> size;

        // Validate the input
        while (size < 5 || size > 15) {
            std::cout << "Please choose a size between 5 and 15: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> size;
        }
        Utils::ClearScreen();
        return size;
    }

    int BattleshipApp::chooseAIType()
    {
        int type;
        std::cout << "Choose the AI type (1 for hunter, 2 for probalistic): ";
        std::cin >> type;

        // Validate the input
        while (type < 0  || type > 3) {
            std::cout << "Please choose a type 1 or 2: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> type;
        }
        Utils::ClearScreen();
        return type;
    }

    void BattleshipApp::DisplayVictoryScreen(const std::string& winningPlayer) {
        // Clear the screen
        Utils::ClearScreen();

        // Display a cool victory message
        std::cout << "Congratulations, " << winningPlayer << "!" << std::endl;
        std::cout << "You are the victor of Battleship!" << std::endl;
        std::cout << R"(
        ___________ 
        \          \
         \  WINNER  \
          \__________\
        )" << std::endl;
    }
}