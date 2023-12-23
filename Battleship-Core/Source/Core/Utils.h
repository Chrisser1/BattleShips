#pragma once
#include <map>
#include <string>
#include <cstdlib>

namespace Utils {
    // Struct to represent a point on the board
    struct Point {
        int x;
        int y;

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Enum for different types of boats
    enum class BoatType {
        Carrier,
        Battleship,
        Cruiser,
        Submarine,
        Destroyer,
        // ... other boat types
    };

    // Map associating each boat type with its size and name
    const std::map<BoatType, std::pair<int, std::string>> BoatSizes = {
        {BoatType::Carrier, {5, "Carrier"}},
        {BoatType::Battleship, {4, "Battleship"}},
        {BoatType::Cruiser, {3, "Cruiser"}},
        {BoatType::Submarine, {3, "Submarine"}},
        {BoatType::Destroyer, {2, "Destroyer"}}
        // ... other boat sizes
    };

    // Struct that keeps the info for each ship
    struct ShipInfo {
        BoatType type;
        int size;
        int health;
        Point start;
        Point end;

        ShipInfo(BoatType t, int s, Point st, Point en) :
            type(t), size(s), health(s), start(st), end(en) {}
    };

    // Enum for the state of a cell on the board
    enum class CellState {
        Empty,
        Ship,
        Hit,
        Miss
    };

    void ClearScreen();
}