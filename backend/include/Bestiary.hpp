#ifndef BESTIARY_HPP
#define BESTIARY_HPP

#include <vector>
#include <memory>
#include <string>
#include "Entities.hpp"

// =========================================================
// STANDARD ENEMY CLASS
// =========================================================
class Enemy : public Entity {
public:
    Enemy(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha);
    
    // Default AI: Attacks the first alive target it sees
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// SPECIALIZED BOSS CLASS (Polymorphism Showcase)
// =========================================================
class BossScrapBaron : public Enemy {
private:
    std::shared_ptr<Entity> lockedTarget; // Memory pointer for his "Target Lock" mechanic
    
public:
    BossScrapBaron();
    
    // Overriding the default AI with complex GDD mechanics
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE ENEMY FACTORY
// =========================================================
class EnemyFactory {
public:
    // Spawns a randomized group of enemies based on the Biome Tier (1, 2, or 3)
    static std::vector<std::shared_ptr<Entity>> SpawnEnemies(int tier);
    
    // Spawns the boss for the specific biome
    static std::vector<std::shared_ptr<Entity>> SpawnBoss(int tier);
};

#endif

