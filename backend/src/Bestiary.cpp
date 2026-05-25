#include "Bestiary.hpp"
#include <iostream>
#include <cstdlib>


std::shared_ptr<Entity> GetRandomAliveTarget(const std::vector<std::shared_ptr<Entity>>& targets) {
    std::vector<std::shared_ptr<Entity>> aliveTargets;
    for (const auto& target : targets) {
        if (target && target->IsAlive()) {
            aliveTargets.push_back(target);
        }
    }
    
    if (aliveTargets.empty()) return nullptr;
    int randomIdx = rand() % aliveTargets.size();
    return aliveTargets[randomIdx];
}

Enemy::Enemy(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha)
    : Entity(n, hp, str, dex, con, intel, wis, cha) {}

void Enemy::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    RegenerateEnergy();
    ProcessStatuses();
    if (!IsAlive()) return;

    std::cout << "\n[ ENEMY TURN - " << name << " ]\n";
    
    // Select a random alive target from the allies
    auto target = GetRandomAliveTarget(allies);
    if (target && target->IsAlive()) {
        int damage = 5 + static_cast<int>(GetTotalStat(STR) * 1.2);
        std::cout << "> " << name << " viciously attacks " << target->GetName() << "!\n";
        target->TakeDamage(damage, BLUNT);
    }
}




BossScrapBaron::BossScrapBaron() 
    : Enemy("Scrap-Baron [BOSS]", 350, 18, 10, 15, 8, 8, 8), lockedTarget(nullptr) {
    maxEnergy = 5;
}

void BossScrapBaron::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    RegenerateEnergy();
    ProcessStatuses();
    if (!IsAlive()) return;

    std::cout << "\n[ BOSS TURN - " << name << " ] (Energy: " << currentEnergy << "/5)\n";

    
    auto target = GetRandomAliveTarget(allies);
    if (!target || !target->IsAlive()) return;

    
    if (currentEnergy >= 3 && lockedTarget != nullptr && lockedTarget->IsAlive()) {
        std::cout << "> " << name << " unleashes CHAIN-RIP on " << lockedTarget->GetName() << "!\n";
        int brutalDamage = 15 + static_cast<int>(GetTotalStat(STR) * 2.5);
        
        lockedTarget->TakeDamage(brutalDamage, SLASHING); 
        lockedTarget->TakeDamage(brutalDamage, SLASHING); 
        lockedTarget->ApplyStatus({BLEED, "Mangled", 3, 10});
        
        SpendEnergy(3);
        lockedTarget = nullptr; 
        return;
    }

    
    if (currentEnergy >= 2 && (lockedTarget == nullptr || !lockedTarget->IsAlive())) {
        lockedTarget = target;
        std::cout << "> " << name << " uses TARGET LOCK! " << lockedTarget->GetName() << " is marked for execution!\n";
        return;
    }

    
    if (currentEnergy < 2) {
        std::cout << "> " << name << " uses REV ENGINE! The chainsaw roars to life (+2 Energy)!\n";
        for (int i = 0; i < 2; i++) RegenerateEnergy();
        return;
    }

    
    std::cout << "> " << name << " uses CLEAVE!\n";
    target->TakeDamage(10 + static_cast<int>(GetTotalStat(STR) * 1.5), SLASHING);
    SpendEnergy(1);
}




std::vector<std::shared_ptr<Entity>> EnemyFactory::SpawnEnemies(int tier) {
    std::vector<std::shared_ptr<Entity>> mob;
    
    
    int numEnemies = (rand() % 2) + 1 + (tier > 1 ? 1 : 0); 
    
    for (int i = 0; i < numEnemies; i++) {
        int roll = rand() % 100;
        
        if (tier == 1) { 
            if (roll < 60) mob.push_back(std::make_shared<Enemy>("Scrap-Hound", 25, 10, 14, 8, 2, 4, 2)); 
            else mob.push_back(std::make_shared<Enemy>("Rogue Drone", 35, 8, 16, 6, 12, 8, 2)); 
        } 
        else if (tier == 2) { 
            if (roll < 50) mob.push_back(std::make_shared<Enemy>("Mutated Crawler", 60, 16, 10, 18, 4, 6, 2)); 
            else mob.push_back(std::make_shared<Enemy>("Rad-Ghoul", 45, 14, 18, 12, 6, 4, 2)); 
        }
        else { 
            if (roll < 50) mob.push_back(std::make_shared<Enemy>("Arcane Sentinel", 100, 12, 14, 16, 22, 18, 10)); 
            else mob.push_back(std::make_shared<Enemy>("Clockwork Guard", 130, 22, 8, 24, 10, 10, 10)); 
        }
    }
    return mob;
}

std::vector<std::shared_ptr<Entity>> EnemyFactory::SpawnBoss(int tier) {
    std::vector<std::shared_ptr<Entity>> bossEncounter;
    
    if (tier == 1) {
        bossEncounter.push_back(std::make_shared<BossScrapBaron>()); 
    } else if (tier == 2) {
        
        bossEncounter.push_back(std::make_shared<Enemy>("The Sand-Goliath [BOSS]", 250, 25, 8, 25, 5, 10, 5)); 
    } else {
        
        bossEncounter.push_back(std::make_shared<Enemy>("Citadel Overseer [FINAL BOSS]", 450, 20, 20, 20, 30, 30, 20)); 
    }
    
    return bossEncounter;
}