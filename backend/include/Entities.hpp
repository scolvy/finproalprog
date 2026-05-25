#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Globals.hpp"
#include "Equipment.hpp"
#include "NetworkManager.hpp"
#include "NetworkUtils.hpp"

class Entity {
protected:
    std::string name;
    int currentHP;
    int maxHP;
    int currentEnergy;
    int maxEnergy;

    
    int level;
    int currentXP;
    int xpToNextLevel;
    int unallocatedStatPoints;

    
    std::map<StatType, int> baseAttributes;
    std::map<EquipSlot, Equipment> loadout; 
    std::vector<StatusEffect> activeEffects;

public:
    Entity(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha);
    virtual ~Entity() = default;

    virtual void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) = 0;
    
    
    virtual void TakeDamage(int amount, DamageType type);
    virtual void Heal(int amount);
    void SpendEnergy(int amount);
    void RegenerateEnergy();
    
    
    int GetTotalStat(StatType type) const; 
    void GainXP(int amount);
    void LevelUp();
    void AllocateStat(StatType type);
    void EquipItem(Equipment item);
    void ApplyStatus(StatusEffect effect);
    void ProcessStatuses();
    void ClearDebuffs(); 
    void ExecuteAttackOnTarget(std::shared_ptr<Entity> target, std::vector<std::shared_ptr<Entity>>& enemies);
    
    void ModifyBaseStat(StatType type, int amount);
    void ModifyMaxHP(int amount);
    void ModifyMaxEnergy(int amount);

    
    bool IsAlive() const;
    std::string GetName() const;
    int GetHP() const;
    int GetMaxHP() const;
    void SetMaxHP(int amount);
    int GetEnergy() const;
    int GetLevel() const;
    int GetUnallocatedPoints() const;
};

class Character : public Entity {
private:
    std::unique_ptr<JobClass> currentJob; 

public:
    Character(std::string n, std::unique_ptr<JobClass> startingJob, int hp, int str, int dex, int con, int intel, int wis, int cha);
    
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
    std::string PromptLocalTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies);
    void ExecuteNetworkTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies, int choice, int targetIdx);
    
    void EvolveJob(std::unique_ptr<JobClass> newJob);
    JobClass* GetCurrentJob() const;
    
};

inline bool IsValidTarget(int index, const std::vector<std::shared_ptr<Entity>>& targets) {
    return (index >= 0 && index < targets.size() && targets[index] && targets[index]->IsAlive());
}



#endif