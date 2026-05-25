#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Globals.hpp"
#include "Equipment.hpp"

class Entity {
protected:
    std::string name;
    int currentHP;
    int maxHP;
    int currentEnergy;
    int maxEnergy;

    // --- NEW PROGRESSION SYSTEM ---
    int level;
    int currentXP;
    int xpToNextLevel;
    int unallocatedStatPoints;

    // Stats are now separated so gear can be swapped without ruining base stats
    std::map<StatType, int> baseAttributes;
    std::map<EquipSlot, Equipment> loadout; 
    std::vector<StatusEffect> activeEffects;

public:
    Entity(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha);
    virtual ~Entity() = default;

    virtual void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) = 0;
    
    // Combat Math
    virtual void TakeDamage(int amount, DamageType type);
    virtual void Heal(int amount);
    void SpendEnergy(int amount);
    void RegenerateEnergy();
    
    // Progression & Inventory Math
    int GetTotalStat(StatType type) const; // Calculates Base + Gear
    void GainXP(int amount);
    void LevelUp();
    void AllocateStat(StatType type);
    void EquipItem(Equipment item);
    void ApplyStatus(StatusEffect effect);
    void ProcessStatuses();
    void ClearDebuffs(); // Specifically needed for Flesh-Mender's 'Purge' skill
    // Event Node Modifiers
    void ModifyBaseStat(StatType type, int amount);
    void ModifyMaxHP(int amount);
    void ModifyMaxEnergy(int amount);

    // Getters
    bool IsAlive() const;
    std::string GetName() const;
    int GetHP() const;
    int GetMaxHP() const;
    int GetEnergy() const;
    int GetLevel() const;
    int GetUnallocatedPoints() const;
};

class Character : public Entity {
private:
    std::unique_ptr<JobClass> currentJob; // The Strategy Pattern pointer

public:
    Character(std::string n, std::unique_ptr<JobClass> startingJob, int hp, int str, int dex, int con, int intel, int wis, int cha);
    
    // Overriding the pure virtual function from Entity
    void TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) override;
    void EvolveJob(std::unique_ptr<JobClass> newJob);
};


#endif