#include "GameManager.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    GameManager* gameEngine = GameManager::GetInstance();

    gameEngine->RunLoop();

    return 0;
}