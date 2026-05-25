#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include <memory>
#include <string>
#include "Entities.hpp"
#include "Map.hpp"
#include "Bestiary.hpp"
#include "EventSystem.hpp"
#include "Algorithms.hpp"
#include "Jobs.hpp"

// The core states of the game engine
enum GameState { 
    STATE_MENU, 
    STATE_CHARACTER_SELECT, 
    STATE_MAP, 
    STATE_COMBAT, 
    STATE_LEVELUP, 
    STATE_GAMEOVER,
    STATE_MERCHANT,
    STATE_CAMP
};

// ENKAPSULASI: Singleton Pattern
class GameManager {
private:
    static GameManager* instance;
    GameState currentState;
    
    // Core Game Data
    std::shared_ptr<Entity> player;
    std::vector<std::shared_ptr<Entity>> party;
    std::vector<std::shared_ptr<Entity>> enemies;
    std::unique_ptr<MapTree> gameMap;
    
    // Progression & Inventory
    int currentTier;
    std::string currentBiome;
    int partyScrap;
    int healthSalves;
    int turnCounter;

    // Private constructor
    GameManager();

    // Internal State Resolvers
    void ResolveNode();
    void HandleCombatTurn();
    void CheckCombatEnd();
public:
    // Prevent cloning
    GameManager(const GameManager& obj) = delete;
    void operator=(const GameManager&) = delete;

    static GameManager* GetInstance();

    // Core Loop
    void RunLoop();
    void SetState(GameState newState);
    
    // UI Rendering
    void ClearScreen();
    void DrawMainMenu();
    void DrawCharacterSelect();
    void DrawMapUI();
    void DrawCombatUI();
    void DrawLevelUpUI();
    void DrawDeathScreen();
    void DrawMerchantUI();
    void DrawCampfireUI();
};

#endif