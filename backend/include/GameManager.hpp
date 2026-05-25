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


class GameManager {
private:
    static GameManager* instance;
    GameState currentState;
    
    
    std::shared_ptr<Entity> player;
    std::vector<std::shared_ptr<Entity>> party;
    std::vector<std::shared_ptr<Entity>> enemies;
    std::unique_ptr<MapTree> gameMap;
    
    
    int currentTier;
    std::string currentBiome;
    int partyScrap;
    int healthSalves;
    int turnCounter;

    
    GameManager();

    
    void ResolveNode();
    void HandleCombatTurn();
    void CheckCombatEnd();
public:
    
    GameManager(const GameManager& obj) = delete;
    void operator=(const GameManager&) = delete;

    static GameManager* GetInstance();

    
    void RunLoop();
    void SetState(GameState newState);
    
    
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