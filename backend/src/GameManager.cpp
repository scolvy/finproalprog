#include "GameManager.hpp"
#include "Map.hpp"
#include <iostream>
#include <cstdlib>

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : currentState(STATE_MENU), currentTier(1), partyScrap(0), healthSalves(1), partySize(1) {
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
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; 
}

void GameManager::SetState(GameState newState) {
    currentState = newState;
}

void GameManager::DrawMainMenu() {
    ClearScreen();
ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << "                       S Y S T E M   I N I T I A L I Z E D                      \n";
    std::cout << "================================================================================\n\n";
    std::cout << "  > Establish connection username: ";
    std::cin >> localUsername;

    ClearScreen();
    std::cout << "================================================================================";
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
    std::cout << "================================================================================";
    std::cout << "================================================================================\n";
    std::cout<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n";
    std::cout << "========================================\n";
    std::cout << " WELCOME, " << localUsername << "\n";
    std::cout << "========================================\n";
    std::cout << " [1] Play Solo (Offline)\n";
    std::cout << " [2] Host Multiplayer Game\n";
    std::cout << " [3] Join Multiplayer Game\n";
    std::cout << "> Choice: ";
    
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        srand(time(0));
        partySize = 1;
        DrawCharacterSelect();
    } 
    else if (choice == 2) {
        std::cout << "Enter total party size (2-3): ";
        std::cin >> partySize;
        std::cout << "Waiting for " << (partySize - 1) << " players to connect...\n";
        
        network.StartHost(8888, partySize - 1);
        network.AcceptClients(partySize - 1);
        
        int rngSeed = time(0);
        srand(rngSeed);
        network.BroadcastMessage("SEED|" + std::to_string(rngSeed));
        
        DrawCharacterSelect();
    } 
    else if (choice == 3) {
        std::string ip;
        std::cout << "Enter Host IP (e.g., 127.0.0.1): ";
        std::cin >> ip;
        
        if (network.ConnectToHost(ip, 8888)) {
            std::cout << "Connected! Waiting for host to start...\n";
            std::string seedMsg = network.ReceiveMessage(network.GetConnections()[0]);
            std::vector<std::string> tokens = NetworkUtils::Split(seedMsg, '|');
            if (tokens.size() > 1 && tokens[0] == "SEED") srand(std::stoi(tokens[1]));
            
            DrawCharacterSelect();
        } else {
            std::cout << "Connection failed.\n";
            exit(0);
        }
    }
}

std::string GameManager::BuildPartySyncMsg() {
    std::string msg = "PARTY|" + std::to_string(currentTier) + "|" + currentBiome + "|" + std::to_string(party.size());
    for (const auto& member : party) {
        int classID = 1;
        if (dynamic_cast<Drifter*>(member->GetCurrentJob())) classID = 2;
        else if (dynamic_cast<Sparkmage*>(member->GetCurrentJob())) classID = 3;
        else if (dynamic_cast<Apothecary*>(member->GetCurrentJob())) classID = 4;
        else if (dynamic_cast<Envoy*>(member->GetCurrentJob())) classID = 5;
        else if (dynamic_cast<Juggernaut*>(member->GetCurrentJob())) classID = 6;
        else if (dynamic_cast<Phantom*>(member->GetCurrentJob())) classID = 7;
        else if (dynamic_cast<Voidcaller*>(member->GetCurrentJob())) classID = 8;
        else if (dynamic_cast<FleshMender*>(member->GetCurrentJob())) classID = 9;
        else if (dynamic_cast<Commander*>(member->GetCurrentJob())) classID = 10;
        else if (dynamic_cast<BloodBrawler*>(member->GetCurrentJob())) classID = 11;
        else if (dynamic_cast<Gunslinger*>(member->GetCurrentJob())) classID = 12;
        else if (dynamic_cast<Stormweaver*>(member->GetCurrentJob())) classID = 13;
        else if (dynamic_cast<PlagueDoctor*>(member->GetCurrentJob())) classID = 14;
        else if (dynamic_cast<Charlatan*>(member->GetCurrentJob())) classID = 15;
        
        msg += "|" + member->GetName() + ":" + std::to_string(classID);
    }
    return msg;
}

void GameManager::ParsePartySync(const std::string& data) {
    party.clear();
    std::vector<std::string> tokens = NetworkUtils::Split(data, '|');
    
    if (tokens.empty() || tokens[0] != "PARTY") return;
    
    currentTier = std::stoi(tokens[1]);
    currentBiome = tokens[2];
    int expectedSize = std::stoi(tokens[3]);
    
    for (int i = 0; i < expectedSize; i++) {
        std::vector<std::string> charData = NetworkUtils::Split(tokens[i + 4], ':');
        std::string pName = charData[0];
        int cID = std::stoi(charData[1]);
        
        std::unique_ptr<JobClass> job;
        if (cID == 1) job = std::make_unique<Scrapper>();
        else if (cID == 2) job = std::make_unique<Drifter>();
        else if (cID == 3) job = std::make_unique<Sparkmage>();
        else if (cID == 4) job = std::make_unique<Apothecary>();
        else if (cID == 5) job = std::make_unique<Envoy>();
        else if (cID == 6) job = std::make_unique<Juggernaut>();
        else if (cID == 7) job = std::make_unique<Phantom>();
        else if (cID == 8) job = std::make_unique<Voidcaller>();
        else if (cID == 9) job = std::make_unique<FleshMender>();
        else if (cID == 10) job = std::make_unique<Commander>();
        else if (cID == 11) job = std::make_unique<BloodBrawler>();
        else if (cID == 12) job = std::make_unique<Gunslinger>();
        else if (cID == 13) job = std::make_unique<Stormweaver>();
        else if (cID == 14) job = std::make_unique<PlagueDoctor>();
        else job = std::make_unique<Charlatan>();
        
        party.push_back(std::make_shared<Character>(pName, std::move(job), 120, 14, 8, 16, 6, 8, 8));
    }
}

void GameManager::DrawCharacterSelect() {
    ClearScreen();
    party.clear();

    auto DrawClassMenu = []() {
        std::cout << "================================================================================\n";
        std::cout << "                      A V A I L A B L E   S Y N D I C A T E S                   \n";
        std::cout << "================================================================================\n";
        std::cout << "  [1] SCRAPPER   - Frontline Brawler (High HP, Melee focus)\n";
        std::cout << "  [2] DRIFTER    - Agile Survivor    (Balanced, Evasion)\n";
        std::cout << "  [3] SPARKMAGE  - Tech Caster       (High Damage, Fragile)\n";
        std::cout << "  [4] APOTHECARY - Combat Medic      (Healing, Party Buffs)\n";
        std::cout << "  [5] ENVOY      - Tactical Leader   (Debuffs, Support)\n";
        std::cout << "--------------------------------------------------------------------------------\n";
        std::cout << "  > Select Class: " << std::flush;
    };

    if (network.IsConnected() && !network.IsHost()) {
        DrawClassMenu();
        int choice;
        std::cin >> choice;

        std::string draftMsg = "DRAFT|" + localUsername + "|" + std::to_string(choice);
        network.SendMessageTo(network.GetConnections()[0], draftMsg);

        std::cout << "\n  Waiting for Host to finalize the party...\n";
        std::string syncMsg = network.ReceiveMessage(network.GetConnections()[0]);
        ParsePartySync(syncMsg);
        
    } else {
        DrawClassMenu();
        int choice;
        std::cin >> choice;

        std::unique_ptr<JobClass> job;
        if (choice == 1) job = std::make_unique<Scrapper>();
        else if (choice == 2) job = std::make_unique<Drifter>();
        else if (choice == 3) job = std::make_unique<Sparkmage>();
        else if (choice == 4) job = std::make_unique<Apothecary>();
        else job = std::make_unique<Envoy>();

        party.push_back(std::make_shared<Character>(localUsername, std::move(job), 120, 14, 8, 16, 6, 8, 8));

        if (network.IsHost()) {
            for (SOCKET clientSocket : network.GetConnections()) {
                std::cout << "\n  Waiting for clients to draft...\n";
                std::string clientMsg = network.ReceiveMessage(clientSocket);
                
                std::vector<std::string> tokens = NetworkUtils::Split(clientMsg, '|');
                if (tokens.size() == 3 && tokens[0] == "DRAFT") {
                    std::string cName = tokens[1];
                    int cID = std::stoi(tokens[2]);
                    clientSockets[cName] = clientSocket; 

                    std::unique_ptr<JobClass> cJob;
                    if (cID == 1) cJob = std::make_unique<Scrapper>();
                    else if (cID == 2) cJob = std::make_unique<Drifter>();
                    else if (cID == 3) cJob = std::make_unique<Sparkmage>();
                    else if (cID == 4) cJob = std::make_unique<Apothecary>();
                    else cJob = std::make_unique<Envoy>();

                    party.push_back(std::make_shared<Character>(cName, std::move(cJob), 120, 14, 8, 16, 6, 8, 8));
                }
            }
            std::string partySync = BuildPartySyncMsg();
            network.BroadcastMessage(partySync);
        }
    }
    player = party[0];
    gameMap = std::make_unique<MapTree>(15);
    SetState(STATE_MAP);
}

void GameManager::ResolveNode() {
    NodeType currentType = gameMap->GetCurrentNode()->type;
    
    if (currentType == COMBAT) {
        enemies = EnemyFactory::SpawnEnemies(currentTier);
        ScaleDifficulty(enemies);
        turnCounter = 1;
        SetState(STATE_COMBAT);
    } 
    else if (currentType == ELITE) {
        if (gameMap->IsAtBoss()) {
            enemies = EnemyFactory::SpawnBoss(currentTier);
        } else {
            enemies = EnemyFactory::SpawnEnemies(currentTier + 1);
        }
        ScaleDifficulty(enemies);
        turnCounter = 1;
        SetState(STATE_COMBAT);
    }
    else if (currentType == EVENT) {
        EventSystem::ResolveRandomEvent(player, partyScrap);
        std::cout << "\n  [ Press Enter to continue... ]";
        std::cin.ignore(); std::cin.get();
    }
    else if (currentType == MERCHANT) {
        SetState(STATE_MERCHANT);
    }
    else if (currentType == CAMP) {
        SetState(STATE_CAMP); 
    }
}

void GameManager::DrawMapUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << " [ STATUS ]  Scrap: " << partyScrap << "  |  Salves: " << healthSalves << "  |  Biome: " << currentBiome << "\n";
    std::cout << " [ PLAYER ]  Level: " << player->GetLevel() << "  |  HP: [" << player->GetHP() << "/" << player->GetMaxHP() << "]\n";
    std::cout << "================================================================================\n\n";
    
    gameMap->PrintAvailablePaths();
    
    if (gameMap->IsAtBoss()) {
        std::cout << "\n  [!] THE BOSS LOOMS AHEAD...\n";
        std::cout << "  Press Enter to face your fate... ";
        if (!network.IsConnected() || network.IsHost()) {
            std::cin.ignore(); std::cin.get();
            if (network.IsHost()) network.BroadcastMessage("MAP|BOSS");
        } else {
            std::string mapMsg = network.ReceiveMessage(network.GetConnections()[0]);
        }
        ResolveNode();
        return;
    }

    if (network.IsConnected() && !network.IsHost()) {
        std::cout << "\n  Waiting for Host to chart a path...\n";
        std::string mapMsg = network.ReceiveMessage(network.GetConnections()[0]);
        std::vector<std::string> tokens = NetworkUtils::Split(mapMsg, '|');
        if (tokens.size() > 1 && tokens[0] == "MAP") {
            if (tokens[1] == "L") gameMap->MoveLeft();
            else if (tokens[1] == "R") gameMap->MoveRight();
            ResolveNode();
        }
    } else {
        bool validChoice = false;
        while (!validChoice) {
            std::cout << "\n  > Enter [L] for Left, [R] for Right, or [H] to Use Salve: " << std::flush;
            char choice;
            std::cin >> choice;
            
            if (choice == 'H' || choice == 'h') {
                if (healthSalves > 0) {
                    healthSalves--;
                    player->Heal(player->GetMaxHP() * 0.50); // Heals 50% max HP
                    std::cout << "  [+] Salve used! Restored HP.\n";
                    std::cout << "  [ PLAYER ]  HP: [" << player->GetHP() << "/" << player->GetMaxHP() << "] | Salves Left: " << healthSalves << "\n";
                } else {
                    std::cout << "  [!] You are out of Health Salves!\n";
                }
            } 
            else if (choice == 'L' || choice == 'l') {
                gameMap->MoveLeft();
                if (network.IsHost()) network.BroadcastMessage("MAP|L");
                validChoice = true;
                ResolveNode();
            } 
            else if (choice == 'R' || choice == 'r') {
                gameMap->MoveRight();
                if (network.IsHost()) network.BroadcastMessage("MAP|R");
                validChoice = true;
                ResolveNode();
            }
        }
    }
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
        if (player->GetLevel() >= 5) {
            std::cout << "\nChoose your path:\n";
            std::cout << "[1] Juggernaut (Tank) | [2] BloodBrawler (Berserker)\n";
            int path; std::cin >> path;
            
            if (path == 1) player->EvolveJob(std::make_unique<Juggernaut>());
            else if (path == 2) player->EvolveJob(std::make_unique<BloodBrawler>());
            
            std::cout << "\nEvolution complete! Press Enter...";
            std::cin.ignore(); std::cin.get();
        } else {
            std::cout << "\n[!] Evolution unavailable (Check Level/Class)\n";
            std::cin.ignore(); std::cin.get();
        }
    }

    else if (choice == 3) {
            if (inventory.empty()) {
                std::cout << "  [!] Your inventory is empty.\n";
                std::cin.ignore(); std::cin.get();
            } else {
                std::cout << "\n  --- INVENTORY ---\n";
                for (size_t i = 0; i < inventory.size(); i++) {
                    int power = inventory[i].statModifiers.empty() ? 0 : inventory[i].statModifiers.begin()->second;
                    std::cout << "  [" << i+1 << "] " << inventory[i].name 
                              << " (+ " << power << ")\n";
                }
                std::cout << "  > Select weapon to equip (0 to cancel): ";
                int wIdx; std::cin >> wIdx;
                
                if (wIdx > 0 && wIdx <= inventory.size()) {
                    std::cout << "\n  --- PARTY ---\n";
                    for (size_t i = 0; i < party.size(); i++) {
                        std::cout << "  [" << i+1 << "] " << party[i]->GetName() << "\n";
                    }
                    std::cout << "  > Select character: ";
                    int cIdx; std::cin >> cIdx;
                    
                    if (cIdx > 0 && cIdx <= party.size()) {
                        party[cIdx-1]->EquipItem(inventory[wIdx-1]);
                        inventory.erase(inventory.begin() + (wIdx - 1));
                    }
                }
                std::cin.ignore(); std::cin.get();
            }
        }
    
    SetState(STATE_MAP);
}

void GameManager::ScaleDifficulty(std::vector<std::shared_ptr<Entity>>& enemies) {
    if (party.size() <= 1) return;
    
    double multiplier = 1.0 + (party.size() * 0.4);
    
    for (auto& enemy : enemies) {
        enemy->SetMaxHP(enemy->GetMaxHP() * multiplier);
        enemy->Heal(enemy->GetMaxHP());
    }
}

Equipment GameManager::GenerateWeaponDrop() {
    int slotRoll = rand() % 3;
    EquipSlot slot = static_cast<EquipSlot>(slotRoll);
    std::string name = "";
    std::string desc = "";
    std::map<StatType, int> mods;
    int power = (currentTier * 2) + (rand() % 4) + 1;

    if (slot == WEAPON) {
        std::vector<std::string> prefixes = {"Rusty", "Scrap", "Plasma", "Heavy", "Volt"};
        std::vector<std::string> types = {"Blade", "Pipe", "Rifle", "Gauntlet", "Driver"};
        name = prefixes[rand() % prefixes.size()] + " " + types[rand() % types.size()];
        desc = "A salvaged offensive weapon tool.";
        StatType stat = STR;
        int statRoll = rand() % 3;
        if (statRoll == 1) stat = DEX;
        else if (statRoll == 2) stat = INTELLIGENCE;
        mods[stat] = power;
    }
    else if (slot == ARMOR) {
        std::vector<std::string> prefixes = {"Reinforced", "Kevlar", "Hazmat", "Chobham", "Plated"};
        std::vector<std::string> types = {"Vest", "Coat", "Exosuit", "Cuirass", "Rig"};
        name = prefixes[rand() % prefixes.size()] + " " + types[rand() % types.size()];
        desc = "Protective gear salvaged from wasteland wrecks.";
        mods[CON] = power * 2;
        if (rand() % 2 == 1) mods[STR] = power / 2;
    }
    else {
        std::vector<std::string> prefixes = {"Ancient", "Glowing", "Magnetic", "Cursed", "Biometric"};
        std::vector<std::string> types = {"Core", "Chip", "Amulet", "Scanner", "Module"};
        name = prefixes[rand() % prefixes.size()] + " " + types[rand() % types.size()];
        desc = "A mysterious functioning piece of retrotech.";
        StatType stat = WIS;
        if (rand() % 2 == 1) stat = CHA;
        mods[stat] = power;
        mods[INTELLIGENCE] = power / 2;
    }

    return Equipment(name, slot, desc, mods);
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
        std::cout << "  Found " << scrapWon << " Scrap.\n";
        
        if (rand() % 100 < 45) { 
            Equipment drop = GenerateWeaponDrop();
            inventory.push_back(drop);
            int dropPower = drop.statModifiers.empty() ? 0 : drop.statModifiers.begin()->second;
            std::cout << "  [!] ITEM DROP: " << drop.name << " (+ " << dropPower << " Stat)\n";
        }
        
        for (auto& member : party) {
            if (member->IsAlive()) member->GainXP(100);
        }

        std::cout << "\n  [ Press Enter to continue... ]";
        std::cin.ignore(); std::cin.get();
        
if (gameMap->IsAtBoss()) {
            ClearScreen();
            
            if (currentTier == 3) {
                std::cout << "================================================================================\n";
                std::cout << "                  V I C T O R Y   A C H I E V E D ! ! !                         \n";
                std::cout << "================================================================================\n\n";
                std::cout << "  CONGRATULATIONS, " << localUsername << "!\n";
                std::cout << "  You and your party have successfully traversed the final sector.\n";
                std::cout << "  The core threat has been neutralized, and the wasteland is secure.\n\n";
                std::cout << "  --- FINAL GAME STATS ---\n";
                std::cout << "  Endgame Level: " << player->GetLevel() << "\n";
                std::cout << "  Total Scrap Retained: " << partyScrap << "\n\n";
                std::cout << "  [ Press Enter to exit to the credits... ]";
                std::cin.get();
                exit(0); 
            }
 
            std::cout << "================================================================================\n";
            std::cout << "                       B O S S   D E F E A T E D !                              \n";
            std::cout << "================================================================================\n\n";
            std::cout << "  You have purged the threat from " << currentBiome << ".\n";
            std::cout << "  Preparing extraction transit to the next sector...\n";
            std::cout << "  [ Press Enter to advance tier... ]";
            std::cin.get();

            currentTier++;
            if (currentTier == 2) currentBiome = "The Rust Dunes";
            else if (currentTier == 3) currentBiome = "Neon Citadel";

            gameMap = std::make_unique<MapTree>(15);
            SetState(STATE_MAP);
            return;
        }
        
        if (player->GetUnallocatedPoints() > 0) SetState(STATE_LEVELUP);
        else SetState(STATE_MAP);
    }
}

void GameManager::DrawCombatUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << "  BIOME: " << currentBiome << " \n";
    std::cout << "  TURN " << turnCounter << " | INIT: DEX Sorted \n";
    std::cout << "================================================================================\n\n";
    
    std::cout << "  >>> [ E N E M I E S ] <<<\n";
    std::cout << "  ----------------------------------------------------------------------\n";
    int eNum = 1;
    for (const auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            std::cout << "   (" << eNum << ") " << enemy->GetName() 
                      << "\t| HP: [ " << enemy->GetHP() << " / " << enemy->GetMaxHP() 
                      << " ]\t| EN: " << enemy->GetEnergy() << "/5\n";
            eNum++;
        }
    }
    
    std::cout << "\n  >>> [ P A R T Y ] <<<\n";
    std::cout << "  ----------------------------------------------------------------------\n";
    for (const auto& member : party) {
        if (member->IsAlive()) {
            std::cout << "   - " << member->GetName() 
                      << "\t| HP: [ " << member->GetHP() << " / " << member->GetMaxHP() 
                      << " ]\t| EN: " << member->GetEnergy() << "/5\n";
        }
    }
    std::cout << "\n================================================================================\n";
}

void GameManager::HandleCombatTurn() {
    std::vector<std::shared_ptr<Entity>> turnOrder;
    for (auto& p : party) turnOrder.push_back(p);
    for (auto& e : enemies) turnOrder.push_back(e);

    Algorithms::QuickSortInitiative(turnOrder, 0, turnOrder.size() - 1);

    std::vector<std::shared_ptr<Entity>> entityParty;
    for (auto& p : party) entityParty.push_back(p);

    for (auto& combatant : turnOrder) {
        if (!combatant->IsAlive()) continue;

        DrawCombatUI();

        Character* pChar = dynamic_cast<Character*>(combatant.get());
        if (pChar) {
            if (!network.IsConnected()) {
                combatant->TakeTurn(entityParty, enemies);
            }
            else if (pChar->GetName() == localUsername) {
                std::string action = pChar->PromptLocalTurn(entityParty, enemies);
                std::string outMsg = "TURN|" + action;
                
                if (network.IsHost()) network.BroadcastMessage(outMsg);
                else network.SendMessageTo(network.GetConnections()[0], outMsg);
                
                std::vector<std::string> tokens = NetworkUtils::Split(action, '|');
                pChar->ExecuteNetworkTurn(entityParty, enemies, std::stoi(tokens[0]), std::stoi(tokens[1]));
            } 
            else {
                std::cout << "\nWaiting for " << pChar->GetName() << " to act...\n";
                std::string turnMsg = "";
                
                if (network.IsHost()) {
                    turnMsg = network.ReceiveMessage(clientSockets[pChar->GetName()]);
                    network.BroadcastMessage(turnMsg);
                } else {
                    turnMsg = network.ReceiveMessage(network.GetConnections()[0]);
                }
                
                std::vector<std::string> tokens = NetworkUtils::Split(turnMsg, '|');
                if (tokens.size() >= 3 && tokens[0] == "TURN") {
                    pChar->ExecuteNetworkTurn(entityParty, enemies, std::stoi(tokens[1]), std::stoi(tokens[2]));
                }
            }
        } else {
            combatant->TakeTurn(enemies, entityParty);
        }

        std::cout << "\n[ Press Enter... ]";
        if (!network.IsConnected() || pChar == nullptr || pChar->GetName() == localUsername) {
            std::cin.ignore(); std::cin.get();
        }
        
        CheckCombatEnd();
        if (currentState != STATE_COMBAT) return; 
    }
    turnCounter++;
}

void GameManager::DrawMerchantUI() {
    ClearScreen();
    std::cout << "  ____________________________________________________________________ \n";
    std::cout << " /                                                                    \\\n";
    std::cout << " |   [ $ ] THE SCRAP-BROKER                                           |\n";
    std::cout << " |   \"Got something that caught your eye, drifter?\"                   |\n";
    std::cout << " \\____________________________________________________________________/\n\n";
    
    std::cout << "  Your Wallet: [ " << partyScrap << " Scrap ]\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  [1] Health Salve ........ 50 Scrap\n";
    std::cout << "  [2] Energy Tonic ........ 75 Scrap\n";
    std::cout << "  [3] Leave\n\n";
    std::cout << "  > Choice: " << std::flush;

    int choice;
    std::cin >> choice;
    if (choice == 1 && partyScrap >= 50) { partyScrap -= 50; healthSalves++; }
    else if (choice == 2 && partyScrap >= 75) { partyScrap -= 75;}
    
    SetState(STATE_MAP);
}

void GameManager::DrawLevelUpUI() {
    ClearScreen();
    std::cout << "================================================================================\n";
    std::cout << "                      A S C E N S I O N   R E A C H E D                     \n";
    std::cout << "================================================================================\n\n";
    
    std::cout << "  You have [ " << player->GetUnallocatedPoints() << " ] Unallocated Points.\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  [1] Strength   (STR) :  " << player->GetTotalStat(STR) << "\n";
    std::cout << "  [2] Dexterity  (DEX) :  " << player->GetTotalStat(DEX) << "\n";
    std::cout << "  [3] Const.     (CON) :  " << player->GetTotalStat(CON) << "\n";
    std::cout << "  [4] Intel.     (INT) :  " << player->GetTotalStat(INTELLIGENCE) << "\n";
    std::cout << "  [5] Wisdom     (WIS) :  " << player->GetTotalStat(WIS) << "\n";
    std::cout << "  [6] Charisma   (CHA) :  " << player->GetTotalStat(CHA) << "\n";
    std::cout << "  ----------------------------------------\n\n";
    
    std::cout << "  > Enter stat number to upgrade: " << std::flush;
    
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
    std::cout << "   T H E  W A S T E L A N D  C L A I M S  A N O T H E R  S O U L    \n";
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

