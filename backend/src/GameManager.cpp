#include "GameManager.hpp"
#include "Map.hpp"
#include <iostream>
#include <cstdlib>


GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : currentState(STATE_MENU), currentTier(1), partyScrap(0), healthSalves(1) {
    currentBiome = "Smog-Choked Slums";
}

GameManager* GameManager::GetInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::ClearScreen() {
    
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; 
}

void GameManager::SetState(GameState newState) {
    currentState = newState;
}

void GameManager::DrawMainMenu() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << R"DELIM(
 _______    ______   _______  _______  _______  ___      _______  _______  _______    _______  ______    _______  ___   _ 
|   _   |  |      | |       ||       ||       ||   |    |   _   ||       ||       |  |       ||    _ |  |       ||   | | |
|  |_|  |  |  _    ||    ___||  _____||   _   ||   |    |  |_|  ||_     _||    ___|  |_     _||   | ||  |    ___||   |_| |
|       |  | | |   ||   |___ | |_____ |  | |  ||   |    |       |  |   |  |   |___     |   |  |   |_||_ |   |___ |      _|
|       |  | |_|   ||    ___||_____  ||  |_|  ||   |___ |       |  |   |  |    ___|    |   |  |    __  ||    ___||     |_ 
|   _   |  |       ||   |___  _____| ||       ||       ||   _   |  |   |  |   |___     |   |  |   |  | ||   |___ |    _  |
|__| |__|  |______| |_______||_______||_______||_______||__| |__|  |___|  |_______|    |___|  |___|  |_||_______||___| |_|
    )DELIM" << std::endl;
    std::cout << "================================================================================\n";
    std::cout << "\n [1] Start Game\n";
    std::cout << " [2] Exit to OS\n\n";
    std::cout << "> Choice: " << std::flush;
    
    int choice;
    std::cin >> choice;
    if (choice == 1) SetState(STATE_CHARACTER_SELECT);
    else exit(0);
}

void GameManager::DrawCharacterSelect() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << " SELECT PARTY SIZE\n";
    std::cout << "================================================================================\n";
    std::cout << " [1] Solo Run (Hard)\n";
    std::cout << " [2] Duo Run (Medium)\n";
    std::cout << " [3] Full Party (Easy)\n\n";
    std::cout << "> Choice (1-3): " << std::flush;
    
    int partySize;
    std::cin >> partySize;
    if(partySize < 1) partySize = 1;
    if(partySize > 3) partySize = 3;

    party.clear(); 
    std::map<std::string, int> classCounts;

    for (int i = 0; i < partySize; i++) {
        ClearScreen();
        std::cout << "================================================================================\n";
        std::cout << " DRAFTING CHARACTER " << (i+1) << " OF " << partySize << "\n";
        std::cout << "================================================================================\n";
        std::cout << " [1] The Scrapper  (STR/CON) - High Armor & Melee\n";
        std::cout << " [2] The Drifter   (DEX/INT) - High Speed & Stealth\n";
        std::cout << " [3] The Sparkmage (INT/WIS) - High Magic Damage\n";
        std::cout << " [4] The Apothecary(WIS/CON) - High Healing & Poison\n";
        std::cout << " [5] The Envoy     (CHA/DEX) - Control & Buffs\n\n";
        std::cout << "> Select Class: " << std::flush;
        
        int choice;
        std::cin >> choice;

        std::string className;
        std::unique_ptr<JobClass> job;
        
        if (choice == 1) { className = "Scrapper"; job = std::make_unique<Scrapper>(); }
        else if (choice == 2) { className = "Drifter"; job = std::make_unique<Drifter>(); }
        else if (choice == 3) { className = "Sparkmage"; job = std::make_unique<Sparkmage>(); }
        else if (choice == 4) { className = "Apothecary"; job = std::make_unique<Apothecary>(); }
        else { className = "Envoy"; job = std::make_unique<Envoy>(); }


        classCounts[className]++;
        std::string finalName = className;
        if (classCounts[className] > 1) {
            finalName += "_" + std::to_string(classCounts[className]);
        }
        
        party.push_back(std::make_shared<Character>(finalName, std::move(job), 120, 14, 8, 16, 6, 8, 8));
    }

    player = party[0]; 
    gameMap = std::make_unique<MapTree>(15);
    SetState(STATE_MAP);
}

void GameManager::DrawMapUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << "| BIOME: " << currentBiome << " | SCRAP: " << partyScrap << " | SALVES: " << healthSalves << " |\n";
    std::cout << "================================================================================\n";
    std::cout << "| PLAYER: HP [" << player->GetHP() << "/" << player->GetMaxHP() << "] | LEVEL: " << player->GetLevel() << "\n";
    std::cout << "================================================================================\n\n";
    
    gameMap->PrintAvailablePaths();
    
    if (gameMap->IsAtBoss()) {
        std::cout << "\n[ Press Enter to face the Boss... ]";
        std::cin.ignore(); std::cin.get();
        ResolveNode();
        return;
    }

    std::cout << "\n> Enter [L] for Left or [R] for Right: " << std::flush;
    char choice;
    std::cin >> choice;
    
    if (choice == 'L' || choice == 'l') gameMap->MoveLeft();
    else if (choice == 'R' || choice == 'r') gameMap->MoveRight();
    
    ResolveNode();
}


void GameManager::DrawMerchantUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << " [ $ ] SCRAP-BROKER: \"What are you buying, drifter?\"\n";
    std::cout << "================================================================================\n";
    std::cout << " Your Scrap: " << partyScrap << "\n\n";
    std::cout << " [1] Health Salve (50 Scrap)\n";
    std::cout << " [2] Energy Tonic (75 Scrap)\n";
    std::cout << " [3] Leave\n";
    std::cout << "> Choice: " << std::flush;

    int choice;
    std::cin >> choice;
    if (choice == 1 && partyScrap >= 50) { partyScrap -= 50; healthSalves++; }
    else if (choice == 2 && partyScrap >= 75) { partyScrap -= 75;}
    
    SetState(STATE_MAP);
}

void GameManager::DrawCampfireUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << " [ R ] CAMPFIRE: You rest by the fire.\n";
    std::cout << "================================================================================\n";
    std::cout << "[1] Restore 30% HP\n";
    std::cout << "[2] Evolve Class (Requires Level 5)\n"; 
    std::cout << "[3] Leave Camp\n";
    std::cout << "> Choice: " << std::flush;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        for (auto& member : party) {
            if (member->IsAlive()) member->Heal(member->GetMaxHP() * 0.30);
        }
    } 
    else if (choice == 2) {
        Character* pChar = dynamic_cast<Character*>(player.get());

        if (pChar && pChar->GetLevel() >= 5) {
            std::cout << "\nChoose your path:\n";
            std::cout << "[1] Juggernaut (Tank) | [2] BloodBrawler (Berserker)\n";
            int path; std::cin >> path;
            
            if (path == 1) pChar->EvolveJob(std::make_unique<Juggernaut>());
            else if (path == 2) pChar->EvolveJob(std::make_unique<BloodBrawler>());
            
            std::cout << "\nEvolution complete! Press Enter...";
            std::cin.ignore(); std::cin.get();
        } else {
            std::cout << "\n[!] Evolution unavailable (Check Level/Class)\n";
            std::cin.ignore(); std::cin.get();
        }
    }
    
    SetState(STATE_MAP);
}

void GameManager::ResolveNode() {
    NodeType currentType = gameMap->GetCurrentNode()->type;
    
    if (currentType == COMBAT) {
        enemies = EnemyFactory::SpawnEnemies(currentTier);
        turnCounter = 1;
        SetState(STATE_COMBAT);
    } 
    else if (currentType == ELITE) {
        if (gameMap->IsAtBoss()) enemies = EnemyFactory::SpawnBoss(currentTier);
        else enemies = EnemyFactory::SpawnEnemies(currentTier + 1);
        turnCounter = 1;
        SetState(STATE_COMBAT);
    }
    else if (currentType == EVENT) {
        EventSystem::ResolveRandomEvent(player, partyScrap);
        std::cout << "\n[ Press Enter to continue... ]";
        std::cin.ignore(); std::cin.get();
    }
    else if (currentType == MERCHANT) {
        SetState(STATE_MERCHANT);
    }
    else if (currentType == CAMP) {
        SetState(STATE_MAP); 
    }
    

}

void GameManager::DrawCombatUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << "| BIOME: " << currentBiome << "                           TURN " << turnCounter << " | INIT: DEX Sorted |\n";
    std::cout << "================================================================================\n";
    
    std::cout << "| [ ENEMIES ]                                                                  |\n";
    int eNum = 1;
    for (const auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            std::cout << "| (" << eNum << ") " << enemy->GetName() << " | HP: [" << enemy->GetHP() << "/" << enemy->GetMaxHP() 
                      << "] | EN: " << enemy->GetEnergy() << "/5\n";
            eNum++;
        }
    }
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "| [ PARTY ]                                                                    |\n";
    for (const auto& member : party) {
        if (member->IsAlive()) {
            std::cout << "| " << member->GetName() << " | HP: [" << member->GetHP() << "/" << member->GetMaxHP() 
                      << "] | EN: " << member->GetEnergy() << "/5\n";
        }
    }
    std::cout << "================================================================================\n";
}

void GameManager::HandleCombatTurn() {
    std::vector<std::shared_ptr<Entity>> turnOrder;
    turnOrder.push_back(player);
    for (auto& e : enemies) turnOrder.push_back(e);

    Algorithms::QuickSortInitiative(turnOrder, 0, turnOrder.size() - 1);

    for (auto& combatant : turnOrder) {
        if (!combatant->IsAlive()) continue;

        DrawCombatUI();

        if (dynamic_cast<Character*>(combatant.get())) {
            std::vector<std::shared_ptr<Entity>> partyRef; 
            partyRef.push_back(player); 
            combatant->TakeTurn(partyRef, enemies);
        } else {

            std::vector<std::shared_ptr<Entity>> partyRef;
            partyRef.push_back(player);
            combatant->TakeTurn(enemies, partyRef);
        }

        std::cout << "\n[ Press Enter... ]";
        std::cin.ignore(); std::cin.get();

        CheckCombatEnd();
        if (currentState != STATE_COMBAT) return; 
    }
    turnCounter++;
}

void GameManager::CheckCombatEnd() {
    bool allPlayersDead = true;
    for (auto& p : party) if (p->IsAlive()) allPlayersDead = false;
    
    if (allPlayersDead) {
        SetState(STATE_GAMEOVER);
        return;
    }

    bool allEnemiesDead = true;
    for (auto& e : enemies) if (e->IsAlive()) allEnemiesDead = false;

    if (allEnemiesDead) {
        std::cout << "\n*** COMBAT CLEAR! ***\n";
        int scrapWon = (rand() % 30) + (currentTier * 20);
        partyScrap += scrapWon;
        std::cout << "Found " << scrapWon << " Scrap.\n";
        
        
        player->GainXP(100); 

        std::cout << "\n[ Press Enter to return to Map... ]";
        std::cin.ignore(); std::cin.get();
        
        if (player->GetUnallocatedPoints() > 0) SetState(STATE_LEVELUP);
        else SetState(STATE_MAP);
    }
}




void GameManager::DrawLevelUpUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << " LEVEL UP! [ Unallocated Points: " << player->GetUnallocatedPoints() << " ] \n";
    std::cout << "================================================================================\n";
    std::cout << "[1] Strength (STR): " << player->GetTotalStat(STR) << "\n";
    std::cout << "[2] Dexterity(DEX): " << player->GetTotalStat(DEX) << "\n";
    std::cout << "[3] Const.   (CON): " << player->GetTotalStat(CON) << "\n";
    std::cout << "[4] Intel.   (INT): " << player->GetTotalStat(INT) << "\n";
    std::cout << "[5] Wisdom   (WIS): " << player->GetTotalStat(WIS) << "\n";
    std::cout << "[6] Charisma (CHA): " << player->GetTotalStat(CHA) << "\n\n";
    std::cout << "> Enter stat number to upgrade: " << std::flush;
    
    int choice;
    std::cin >> choice;
    StatType chosenStat = static_cast<StatType>(choice - 1);
    
    player->AllocateStat(chosenStat);
    
    if (player->GetUnallocatedPoints() == 0) SetState(STATE_MAP);
}

void GameManager::DrawDeathScreen() {
    ClearScreen();
    std::cout << "\n\n";
    std::cout << "========================================================================\n";
    std::cout << "   T H E   W A S T E L A N D   C L A I M S   A N O T H E R   S O U L    \n";
    std::cout << "========================================================================\n";
    std::cout << "  __   __  _______  __   __    ______   ___   _______  ______  \n";
    std::cout << "  \\ \\ / / |       ||  | |  |  |      | |   | |       ||      | \n";
    std::cout << "   \\   /  |   _   ||  | |  |  |  _    ||   | |    ___||  _    |\n";
    std::cout << "    | |   |  | |  ||  |_|  |  | | |   ||   | |   |___ | | |   |\n";
    std::cout << "    | |   |  |_|  ||       |  | |_|   ||   | |    ___|| |_|   |\n";
    std::cout << "    | |   |       ||       |  |       ||   | |   |___ |       |\n";
    std::cout << "    |_|   |_______||_______|  |______| |___| |_______||______| \n";
    std::cout << "========================================================================\n";
    std::cout << "\n";
    std::cout << "     The wasteland has claimed another wanderer.\n";
    std::cout << "     Final Level: " << player->GetLevel() << "\n";
    std::cout << "     Total Scrap: " << partyScrap << "\n\n";
    std::cout << "     [ Press Enter to exit to the void... ]" << std::flush;
    
    std::cin.ignore();
    std::cin.get();
    exit(0);
}




void GameManager::RunLoop() {
    bool isRunning = true;

    while (isRunning) {
        switch (currentState) {
            case STATE_MENU:             DrawMainMenu(); break;
            case STATE_CHARACTER_SELECT: DrawCharacterSelect(); break;
            case STATE_MAP:              DrawMapUI(); break;
            case STATE_COMBAT:           HandleCombatTurn(); break;
            case STATE_LEVELUP:          DrawLevelUpUI(); break;
            case STATE_MERCHANT:         DrawMerchantUI(); break;
            case STATE_CAMP:             DrawCampfireUI(); break;
            case STATE_GAMEOVER:
                ClearScreen();
                DrawDeathScreen();
                break;
                
        }
    }
}