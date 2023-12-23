#include "BattleshipAI.h"

BattleshipAI::BattleshipAI(int type)
{
    // If the type is 1, then the AI works by hunt / target else it's probalistic 
    if (type == 1) {
        isProbalistic = false;
    }
    else
    {
        isProbalistic = true;
    }
}

void BattleshipAI::MakeAttack(Player& opponent) {
    Utils::Point attackPoint;
    std::vector<std::vector<int>> probabilityMap;
    if (isProbalistic) {
        probabilityMap = GenerateProbabilityMap(opponent);
        attackPoint = FindHighestProbability(probabilityMap, opponent.GetBoardSize());
    }
    else {
        if (isHunting) {
            bool validAttack = false;

            while (!validAttack) {
                attackPoint.x = rand() % opponent.GetBoardSize();
                attackPoint.y = rand() % opponent.GetBoardSize();

                if (!opponent.getBoard().HasAlreadyAttacked(attackPoint)) {
                    validAttack = true;
                }
            }
        }
        else { // In Target mode
            if (!targetSquares.empty()) {
                attackPoint = targetSquares.back(); // Take the last target square
                targetSquares.pop_back(); // Remove the used target square
            }
            else {
                isHunting = true; // If target list is empty, switch back to Hunt mode
                MakeAttack(opponent); // Go back to hunting
                return;
            }
        }
    }

    opponent.getBoard().AttackCoordinate(attackPoint);
    if (isProbalistic)
        DisplayHeatmap(probabilityMap);
    std::cout << "The AI attacked: (" << attackPoint.x << "," << attackPoint.y << ")" << std::endl;
    opponent.getBoard().DisplayBoard(false);

    // Check if the attack was a hit and switch to Target mode
    if (opponent.getBoard().GetCellState(attackPoint) == Utils::CellState::Hit && !isProbalistic) {
        isHunting = false;
        AddAdjacentSquares(attackPoint, opponent);
    }

    // Wait for the player to continue
    std::string confirmation;
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
    std::getline(std::cin, confirmation);
    Utils::ClearScreen();
}

void BattleshipAI::AddAdjacentSquares(const Utils::Point& point, Player& opponent) {
    // Define the directions: up, down, left, right
    const std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {-1, 0}, {1, 0} };

    // Add adjacent squares to the target list (above, below, left, right)
    for (const auto& [dx, dy] : directions) {
        int x = point.x + dx;
        int y = point.y + dy;

        if (x >= 0 && x < opponent.GetBoardSize() && y >= 0 && y < opponent.GetBoardSize()) {
            Utils::Point adjacentPoint = { x, y };
            if (!opponent.getBoard().HasAlreadyAttacked(adjacentPoint) && std::find(targetSquares.begin(), targetSquares.end(), adjacentPoint) == targetSquares.end()) {
                targetSquares.push_back(adjacentPoint);
            }
        }
    }
}

std::vector<std::vector<int>> BattleshipAI::GenerateProbabilityMap(Player& opponent) {
    std::vector<std::vector<int>> probMap(opponent.GetBoardSize(), std::vector<int>(opponent.GetBoardSize(), 0));

    for (const Utils::ShipInfo& ship : opponent.getBoard().GetShips()) {
        if (ship.health == 0)
            continue;

        int useSize = ship.size - 1;

        for (int x = 0; x < opponent.GetBoardSize(); ++x) {
            for (int y = 0; y < opponent.GetBoardSize(); ++y) {
                if (!opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y))) {
                    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> endpoints = CalculateEndpoints(x, y, useSize, opponent.GetBoardSize());

                    for (auto& endPoints : endpoints) {
                        int startX = endPoints.first.first;
                        int startY = endPoints.first.second;
                        int endX = endPoints.second.first;
                        int endY = endPoints.second.second;

                        if (CanPlaceShip(opponent.getBoard(), Utils::Point(startX, startY), Utils::Point(endX, endY))) {
                            UpdateProbabilityMap(probMap, startX, startY, endX, endY);
                        }
                    }
                }
                // increase probability of attacking squares near successful hits and set miss to 0
                UpdateProbabilities(probMap, x, y, opponent);
            }
        }
    }

    return probMap;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> BattleshipAI::CalculateEndpoints(int x, int y, int useSize, int boardSize) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> endpoints;

    if (x - useSize >= 0) {
        endpoints.push_back(std::make_pair(std::make_pair(x - useSize, y), std::make_pair(x + 1, y + 1)));
    }
    if (x + useSize <= boardSize - 1) {
        endpoints.push_back(std::make_pair(std::make_pair(x, y), std::make_pair(x + useSize + 1, y + 1)));
    }
    if (y - useSize >= 0) {
        endpoints.push_back(std::make_pair(std::make_pair(x, y - useSize), std::make_pair(x + 1, y + 1)));
    }
    if (y + useSize <= boardSize - 1) {
        endpoints.push_back(std::make_pair(std::make_pair(x, y), std::make_pair(x + 1, y + useSize + 1)));
    }

    return endpoints;
}

void BattleshipAI::UpdateProbabilities(std::vector<std::vector<int>>& probMap, int x, int y, Player& opponent)
{
    // increase probability of attacking squares near successful hits
    int boardSize = opponent.GetBoardSize();
    if (opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y)) &&
        opponent.getBoard().IsOccupied(Utils::Point(x, y)) && 
        !opponent.getBoard().HasSunkShip(Utils::Point(x, y))) {

        if (x + 1 <= boardSize && !opponent.getBoard().HasAlreadyAttacked(Utils::Point(x + 1, y))) {
            if (x - 1 >= 0 && !opponent.getBoard().HasSunkShip(Utils::Point(x - 1, y)) &&
                opponent.getBoard().HasAlreadyAttacked(Utils::Point(x - 1, y)) &&
                opponent.getBoard().IsOccupied(Utils::Point(x - 1, y))) {
                probMap[x + 1][y] += 15;
            }
            else {
                probMap[x + 1][y] += 10;
            }
        }

        if (x - 1 >= 0 && !opponent.getBoard().HasAlreadyAttacked(Utils::Point(x - 1, y))) {
            if (x + 1 <= boardSize && !opponent.getBoard().HasSunkShip(Utils::Point(x + 1, y)) &&
                opponent.getBoard().HasAlreadyAttacked(Utils::Point(x + 1, y)) &&
                opponent.getBoard().IsOccupied(Utils::Point(x + 1, y))) {
                probMap[x - 1][y] += 15;
            }
            else {
                probMap[x - 1][y] += 10;
            }
        }

        if (y + 1 <= boardSize && !opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y + 1))) {
            if (y - 1 >= 0 && !opponent.getBoard().HasSunkShip(Utils::Point(x, y - 1)) &&
                opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y - 1)) &&
                opponent.getBoard().IsOccupied(Utils::Point(x, y - 1))) {
                probMap[x][y + 1] += 15;
            }
            else {
                probMap[x][y + 1] += 10;
            }
        }

        if (y - 1 >= 0 && !opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y - 1))) {
            if (y + 1 <= boardSize && !opponent.getBoard().HasSunkShip(Utils::Point(x, y + 1)) &&
                opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y + 1)) &&
                opponent.getBoard().IsOccupied(Utils::Point(x, y + 1))) {
                probMap[x][y - 1] += 15;
            }
            else {
                probMap[x][y - 1] += 10;
            }
        }
    }
    // decrease probability for misses to zero
    else if (opponent.getBoard().HasAlreadyAttacked(Utils::Point(x, y)) &&
        !opponent.getBoard().IsOccupied(Utils::Point(x, y))) {
        probMap[x][y] = 0;
    }
}

bool BattleshipAI::CanPlaceShip(Board& board, const Utils::Point& start, const Utils::Point& end) {
    for (int i = start.x; i < end.x; ++i) {
        for (int j = start.y; j < end.y; ++j) {
            if (board.HasAlreadyAttacked(Utils::Point(i, j))) {
                return false;
            }
        }
    }
    return true;
}

void BattleshipAI::UpdateProbabilityMap(std::vector<std::vector<int>>& probMap, int startX, int startY, int endX, int endY) {
    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            probMap[x][y]++;
        }
    }
}

void BattleshipAI::DisplayHeatmap(const std::vector<std::vector<int>>& heatmap) {
    std::cout << "Heatmap of the AI's thoughts: " << std::endl;
    for (int x = 0; x < heatmap.size(); ++x) {
        for (int y = 0; y < heatmap[x].size(); ++y) {
            std::cout << std::setw(4) << heatmap[y][x]; // Set width to 4 characters
        }
        std::cout << std::endl;
    }
}

Utils::Point BattleshipAI::FindHighestProbability(const std::vector<std::vector<int>>& probabilityMap, int boardSize) const
{
    int maxProbability = 0;
    Utils::Point highestProbabilityPoint;

    for (int x = 0; x < boardSize; ++x) {
        for (int y = 0; y < boardSize; ++y) {
            if (probabilityMap[x][y] > maxProbability) {
                maxProbability = probabilityMap[x][y];
                highestProbabilityPoint.x = x;
                highestProbabilityPoint.y = y;
            }
        }
    }

    return highestProbabilityPoint;
}

void BattleshipAI::PlaceShips(Player& player) {
    for (const auto& boat : Utils::BoatSizes) {
        Utils::BoatType shipType = boat.first;
        int size = boat.second.first; 
        std::string name = boat.second.second;

        bool shipPlaced = false;

        while (!shipPlaced) {
            Utils::Point startPos;
            Utils::Point endPos;
            bool isVertical = (rand() % 2 == 0); // Randomly choose vertical or horizontal

            startPos.x = rand() % player.GetBoardSize();
            startPos.y = rand() % player.GetBoardSize();

            if (isVertical) {
                endPos.x = startPos.x;
                endPos.y = startPos.y + size - 1; // We subtract one to have the correct size
            }
            else
            {
                endPos.x = startPos.x + size -1;
                endPos.y = startPos.y;
            }

            if (player.getBoard().PlaceShip(Utils::ShipInfo(shipType, size, startPos, endPos), true)) {
                shipPlaced = true;
            }
        }
    }
}
