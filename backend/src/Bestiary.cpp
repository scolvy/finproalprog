#include "Bestiary.hpp"
#include <iostream>
#include <cstdlib>

// =========================================================
// STANDARD ENEMY AI
// =========================================================
Enemy::Enemy(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha)
    : Entity(n, hp, str, dex, con, intel, wis, cha) {}

void Enemy::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    RegenerateEnergy();
    ProcessStatuses();
    if (!IsAlive()) return;

    std::cout << "\n[ ENEMY TURN - " << name << " ]\n";
    
    // Default AI: Find the player and attack
    for (auto& player : enemies) {
        if (player->IsAlive()) {
            int damage = 5 + static_cast<int>(GetTotalStat(STR) * 1.2);
            std::cout << "> " << name << " viciously attacks " << player->GetName() << "!\n";
            player->TakeDamage(damage, BLUNT);
            break;
        }
    }
}

// =========================================================
// TIER 1 BOSS: THE SCRAP-BARON
// =========================================================
BossScrapBaron::BossScrapBaron() 
    : Enemy("Scrap-Baron [BOSS]", 350, 18, 10, 15, 8, 8, 8), lockedTarget(nullptr) {
    maxEnergy = 5;
}

void BossScrapBaron::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    RegenerateEnergy();
    ProcessStatuses();
    if (!IsAlive()) return;

    std::cout << "\n[ BOSS TURN - " << name << " ] (Energy: " << currentEnergy << "/5)\n";

    // Grab a reference to the player (since it's a solo run, they are enemies[0])
    auto player = enemies[0];
    if (!player || !player->IsAlive()) return;

    // MECHANIC 1: CHAIN-RIP (Ultimate Move - 3 Energy)
    if (currentEnergy >= 3 && lockedTarget != nullptr) {
        std::cout << "> " << name << " unleashes CHAIN-RIP on " << lockedTarget->GetName() << "!\n";
        int brutalDamage = 15 + static_cast<int>(GetTotalStat(STR) * 2.5);
        
        lockedTarget->TakeDamage(brutalDamage, SLASHING); // Hit 1
        lockedTarget->TakeDamage(brutalDamage, SLASHING); // Hit 2
        lockedTarget->ApplyStatus({BLEED, "Mangled", 3, 10});
        
        SpendEnergy(3);
        lockedTarget = nullptr; // Reset lock after execution
        return;
    }

    // MECHANIC 2: TARGET LOCK (0 Energy)
    if (currentEnergy >= 2 && lockedTarget == nullptr) {
        lockedTarget = player;
        std::cout << "> " << name << " uses TARGET LOCK! " << lockedTarget->GetName() << " is marked for execution!\n";
        return;
    }

    // MECHANIC 3: REV ENGINE (0 Energy)
    if (currentEnergy < 2) {
        std::cout << "> " << name << " uses REV ENGINE! The chainsaw roars to life (+2 Energy)!\n";
        for (int i = 0; i < 2; i++) RegenerateEnergy();
        return;
    }

    // MECHANIC 4: CLEAVE (1 Energy)
    std::cout << "> " << name << " uses CLEAVE!\n";
    player->TakeDamage(10 + static_cast<int>(GetTotalStat(STR) * 1.5), SLASHING);
    SpendEnergy(1);
}

// =========================================================
// ENEMY FACTORY GENERATOR
// =========================================================
std::vector<std::shared_ptr<Entity>> EnemyFactory::SpawnEnemies(int tier) {
    std::vector<std::shared_ptr<Entity>> mob;
    
    // Spawn 1 to 3 enemies based on RNG and Tier
    int numEnemies = (rand() % 2) + 1 + (tier > 1 ? 1 : 0); 
    
    for (int i = 0; i < numEnemies; i++) {
        int roll = rand() % 100;
        
        if (tier == 1) { // Slums
            if (roll < 60) mob.push_back(std::make_shared<Enemy>("Scrap-Hound", 25, 10, 14, 8, 2, 4, 2)); // Was 45
            else mob.push_back(std::make_shared<Enemy>("Rogue Drone", 35, 8, 16, 6, 12, 8, 2)); // Was 60
        } 
        else if (tier == 2) { // Wastes
            if (roll < 50) mob.push_back(std::make_shared<Enemy>("Mutated Crawler", 60, 16, 10, 18, 4, 6, 2)); // Was 120
            else mob.push_back(std::make_shared<Enemy>("Rad-Ghoul", 45, 14, 18, 12, 6, 4, 2)); // Was 90
        }
        else { // Citadel
            if (roll < 50) mob.push_back(std::make_shared<Enemy>("Arcane Sentinel", 100, 12, 14, 16, 22, 18, 10)); // Was 200
            else mob.push_back(std::make_shared<Enemy>("Clockwork Guard", 130, 22, 8, 24, 10, 10, 10)); // Was 250
        }
    }
    return mob;
}

std::vector<std::shared_ptr<Entity>> EnemyFactory::SpawnBoss(int tier) {
    std::vector<std::shared_ptr<Entity>> bossEncounter;
    
    if (tier == 1) {
        bossEncounter.push_back(std::make_shared<BossScrapBaron>()); // (Change his HP to 150 inside his constructor)
    } else if (tier == 2) {
        // Placeholder for Tier 2 Boss
        bossEncounter.push_back(std::make_shared<Enemy>("The Sand-Goliath [BOSS]", 250, 25, 8, 25, 5, 10, 5)); // Was 600
    } else {
        // Placeholder for Tier 3 Boss
        bossEncounter.push_back(std::make_shared<Enemy>("Citadel Overseer [FINAL BOSS]", 450, 20, 20, 20, 30, 30, 20)); // Was 1000
    }
    
    return bossEncounter;
}