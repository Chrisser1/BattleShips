#include "Player.h"

Player::Player(int boardSize) : playerBoard(boardSize) {
    // Initialize the player's board with the given size
}

void Player::MakeAttack(Player& opponent) {
    Utils::Point attackPoint;
    bool validInput = false;

    // Loop until valid input is provided for attack coordinates
    while (!validInput) {
        std::cout << "Enter attack coordinates (x y): ";
        if (!(std::cin >> attackPoint.x >> attackPoint.y)) {
            std::cout << "Invalid input. Please enter numeric values for x and y." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Check if attack coordinates are within bounds
        if (attackPoint.x < 0 || attackPoint.x >= playerBoard.GetBoardSize() ||
            attackPoint.y < 0 || attackPoint.y >= playerBoard.GetBoardSize()) {
            std::cout << "Invalid coordinates. Please enter values within the board size." << std::endl;
            continue;
        }

        // Check if the point has already been shot at
        if (opponent.playerBoard.HasAlreadyAttacked(attackPoint)) {
            std::cout << "You've already shot at this position. Choose another position." << std::endl;
            continue;
        }

        // Valid input received
        validInput = true;
    }

    // Perform an attack on the opponent's board
    opponent.playerBoard.AttackCoordinate(attackPoint);
}


bool Player::AllShipsSunk()
{
    return playerBoard.AllShipsSunk();
}

bool Player::PlaceShip(Utils::BoatType shipType, Utils::Point pos, bool isVertical) {
    // Get the size of the boat
    int size = Utils::BoatSizes.at(shipType).first;

    Utils::Point endPos;
    if (isVertical) {
        // Calculate end position for vertical placement
        endPos = { pos.x, pos.y + size - 1 };
    }
    else {
        // Calculate end position for horizontal placement
        endPos = { pos.x + size - 1, pos.y };
    }

    // Create a ShipInfo object and place it on the player's board
    Utils::ShipInfo shipInfo(shipType, size, pos, endPos);
    return playerBoard.PlaceShip(shipInfo, false);
}

void Player::PlaceShips() {
    for (const auto& boat : Utils::BoatSizes) {
        std::cout << "Current player: " << playerName << std::endl;
        PlaceSingleShip(boat);
        Utils::ClearScreen();
    }
}

void Player::DisplayPlayerBoard() const {
    // Display the player's own board
    playerBoard.DisplayBoard(false);
}

void Player::SetPlayerName(std::string name)
{
    playerName = name;
}

void Player::PlaceSingleShip(const std::pair<Utils::BoatType, std::pair<int, std::string>>& boat) {
    Utils::BoatType shipType = boat.first;
    int size = boat.second.first;
    std::string name = boat.second.second;

    std::cout << "Placing " << name << " (size " << size << ")" << std::endl;
    playerBoard.DisplayBoard(true);

    bool shipPlaced = false;

    while (!shipPlaced) {
        Utils::Point pos;
        bool isVertical;
        bool validInput = false;

        validInput = GetValidPositionInput(pos);
        if (!validInput) continue;

        validInput = GetVerticalPlacementInput(isVertical);
        if (!validInput) continue;

        shipPlaced = PlaceShip(shipType, pos, isVertical);

        if (shipPlaced) {
            playerBoard.DisplayBoard(true);
        }
    }
}

bool Player::GetValidPositionInput(Utils::Point& pos) {
    std::cout << "Enter starting position (x, y): ";
    if (!(std::cin >> pos.x >> pos.y)) {
        std::cout << "Invalid input. Please enter numeric values for x and y." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    if (!playerBoard.IsValidCoordinate(pos)) {
        std::cout << "Invalid position. Please enter valid coordinates within the board size." << std::endl;
        return false;
    }

    return true;
}

bool Player::GetVerticalPlacementInput(bool& isVertical) {
    char choice;
    std::cout << "Vertical placement? (Y/N): ";
    std::cin >> choice;

    if (std::toupper(choice) == 'Y') {
        isVertical = true;
    }
    else if (std::toupper(choice) == 'N') {
        isVertical = false;
    }
    else {
        std::cout << "Invalid input. Please enter Y or N for vertical placement." << std::endl;
        return false;
    }

    return true;
}
