#include "GameManager.hpp"
#include <iostream>

int main() {
    GameManager* game = GameManager::GetInstance();
    game->RunLoop();
    return 0;
}