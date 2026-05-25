#ifndef BESTIARY_HPP
#define BESTIARY_HPP

#include <vector>
#include <memory>
#include <string>
#include "Entities.hpp"




class Enemy : public Entity {
public:
    Enemy(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha);
    
    
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
};




class BossScrapBaron : public Enemy {
private:
    std::shared_ptr<Entity> lockedTarget; 
    
public:
    BossScrapBaron();
    
    
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
};




class EnemyFactory {
public:
    
    static std::vector<std::shared_ptr<Entity>> SpawnEnemies(int tier);
    
    
    static std::vector<std::shared_ptr<Entity>> SpawnBoss(int tier);
};

#endif

