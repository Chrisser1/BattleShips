#include "Board.h"

Board::Board(int size) : boardSize(size) {
    // Initialize the grid with empty cells
    grid.resize(size, std::vector<Utils::CellState>(size, Utils::CellState::Empty));
}

bool Board::IsValidCoordinate(Utils::Point point) const {
    return point.x >= 0 && point.x < boardSize && point.y >= 0 && point.y < boardSize;
}

bool Board::IsOccupied(Utils::Point point) const
{
    if (grid[point.x][point.y] == Utils::CellState::Ship || grid[point.x][point.y] == Utils::CellState::Hit)
        return true;
    // If not occupied by a ship, return false
    return false;
}

bool Board::CollidesWithShip(Utils::Point start, Utils::Point end) const
{
    Utils::Point currentPoint;
    for (int x = start.x; x <= end.x; x++)
    {
        for (int y = start.y; y <= end.y; y++)
        {
            currentPoint = Utils::Point{ x, y };
            if (IsOccupied(currentPoint))
            {
                return true;
            }
        }
    }

    // If there is no collision return false
    return false;
}

void Board::AttackCoordinate(Utils::Point point)
{
    if (!IsValidCoordinate(point)) {
        return;
    }

    // Update ship info if attacked cell contains a ship
    if (IsOccupied(point)) {
        // Find the ship and reduce its health
        for (auto ship = ships.begin(); ship != ships.end(); ++ship) {
            if (point.x >= ship->start.x && point.x <= ship->end.x &&
                point.y >= ship->start.y && point.y <= ship->end.y) {
                ship->health--; // Reduce health of the ship
                break;
            }
        }
        // Set the cellstate to hit
        grid[point.x][point.y] = Utils::CellState::Hit;
    }
    else {
        // If we dont hit a ship, set the cellstate to miss
        grid[point.x][point.y] = Utils::CellState::Miss;
    }
    Utils::ClearScreen();
}

bool Board::HasSunkShip(Utils::Point pos) {
    // Get the ship at the specified position
    Utils::ShipInfo* ship = GetShipAtPosition(pos);

    // If no ship exists at the position or the ship has no health remaining, it's sunk
    return (ship && ship->health == 0);
}

Utils::ShipInfo* Board::GetShipAtPosition(Utils::Point pos)
{
    // Iterate through all the ships on the board
    for (Utils::ShipInfo& ship : ships) {
        // Check if the position is within the ship's occupied positions
        for (Utils::Point occupiedPos : GetOccupiedPositions(ship)) {
            if (occupiedPos == pos) {
                // Return a pointer to the ship if it occupies the given position
                return &ship;
            }
        }
    }
    return nullptr; // Return nullptr if no ship occupies the given position
}

bool Board::PlaceShip(Utils::ShipInfo ship, bool isAI) {
    // Check if the points are valid
    if (!IsValidCoordinate(ship.start) || !IsValidCoordinate(ship.end)) {
        if (!isAI)
            std::cout << "Ship placement out of bounds! Try a different position." << std::endl;
        return false;
    }

    // Check for overlap with other ships
    if (CollidesWithShip(ship.start, ship.end)) {
        if (!isAI)
            std::cout << "Ship collides with another ship! Try a different position." << std::endl;
        return false;
    }

    // Update grid cells to represent the ship
    for (int x = ship.start.x; x <= ship.end.x; ++x) {
        for (int y = ship.start.y; y <= ship.end.y; y++)
        {
            grid[x][y] = Utils::CellState::Ship;
        }
    }
    
    // Add the ship to ships
    ships.emplace_back(ship);

    // Return true if the ship got placed
    return true;
}

bool Board::AllShipsSunk() const {
    for (auto ship = ships.begin(); ship != ships.end(); ++ship) {
        if (ship->health > 0)
        {
            return false;
        }
    }
    return true;
}

void Board::DisplayBoard(bool showShips) const {
    // Display current ships that you have killed
    if (!showShips) {
        std::cout << "Killed ships:" << std::endl;
        for (const auto& ship : ships) {
            if (ship.health == 0) {
                std::cout << "Type: " << Utils::BoatSizes.at(ship.type).second << " Size: " << ship.size << std::endl;
            }
        }
    }

    // Display grid cells, showing ships, hits, and misses
    const int boardSize = static_cast<int>(grid.size());

    // Display x coords
    std::cout << "    ";
    for (int x = 0; x < boardSize; x++)
    {
        std::cout << std::setw(4) << x;
    }
    std::cout << std::endl; // Move to the next row

    // Display everything else
    for (int y = 0; y < boardSize; ++y) {
        std::cout << std::setw(4) << y; // Display y coords
        for (int x = 0; x < boardSize; ++x) {
            switch (grid[x][y]) {
            case Utils::CellState::Empty:
                std::cout << std::setw(4) << "-"; // Empty cell
                break;
            case Utils::CellState::Ship:
                if (showShips) {
                    std::cout << std::setw(4) << "S"; // Ship present
                }
                else {
                    std::cout << std::setw(4) << "-";
                }
                break;
            case Utils::CellState::Hit:
                std::cout << std::setw(4) << "X"; // Ship hit
                break;
            case Utils::CellState::Miss:
                std::cout << std::setw(4) << "O"; // Missed shot
                break;
            }
        }
        std::cout << std::endl; // Move to the next row
    }
}

std::vector<Utils::Point> Board::GetOccupiedPositions(Utils::ShipInfo ship) const
{
    std::vector<Utils::Point> positions;
    int xDelta = (ship.start.x == ship.end.x) ? 0 : ((ship.start.x < ship.end.x) ? 1 : -1);
    int yDelta = (ship.start.y == ship.end.y) ? 0 : ((ship.start.y < ship.end.y) ? 1 : -1);

    int x = ship.start.x;
    int y = ship.start.y;

    positions.push_back({ x, y });

    for (int i = 1; i < ship.size; ++i) {
        x += xDelta;
        y += yDelta;
        positions.push_back({ x, y });
    }

    return positions;
}

bool Board::HasAlreadyAttacked(Utils::Point pos)
{
    return GetCellState(pos) == Utils::CellState::Hit || GetCellState(pos) == Utils::CellState::Miss;
}
