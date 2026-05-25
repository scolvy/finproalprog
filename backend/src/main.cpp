#include "GameManager.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

int main() {
    // 1. Seed the RNG for the Procedural Map, Enemy Generation, and Events
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2. Boot up the Singleton Game Engine
    GameManager* gameEngine = GameManager::GetInstance();

    // 3. Launch the State Machine
    gameEngine->RunLoop();

    return 0;
}