#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <ctime>
#include "Entities.hpp"
#include "Map.hpp"
#include "Bestiary.hpp"
#include "EventSystem.hpp"
#include "Algorithms.hpp"
#include "Jobs.hpp"
#include "NetworkManager.hpp"
#include "NetworkUtils.hpp"

enum GameState { 
    STATE_MENU, STATE_CHARACTER_SELECT, STATE_MAP, 
    STATE_COMBAT, STATE_LEVELUP, STATE_GAMEOVER,
    STATE_MERCHANT, STATE_CAMP
};

class GameManager {
private:
    static GameManager* instance;
    GameState currentState;
    
    std::shared_ptr<Character> player;
    std::vector<std::shared_ptr<Character>> party;
    std::vector<std::shared_ptr<Entity>> enemies;
    std::unique_ptr<MapTree> gameMap;
    
    int currentTier;
    std::string currentBiome;
    int partyScrap;
    int healthSalves;
    int turnCounter;
    int partySize;

    std::string localUsername;
    NetworkManager network;
    std::map<std::string, SOCKET> clientSockets; 

    std::vector<Equipment> inventory;
    Equipment GenerateWeaponDrop();

    GameManager();

    void ResolveNode();
    void HandleCombatTurn();
    void CheckCombatEnd();
    std::string BuildPartySyncMsg();
    void ParsePartySync(const std::string& data);

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
    void ScaleDifficulty(std::vector<std::shared_ptr<Entity>>& enemies);
};

#endif