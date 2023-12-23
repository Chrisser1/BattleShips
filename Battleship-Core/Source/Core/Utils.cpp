#include "Utils.h"

void Utils::ClearScreen()
{
    // Clearing the terminal screen using system-specific commands
    #ifdef _WIN32
        // For Windows
            system("cls");
    #else
        // For Unix/Linux
            system("clear");
    #endif
}
