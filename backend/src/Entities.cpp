#include "Entities.hpp"
#include "Algorithms.hpp"


Entity::Entity(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha) 
    : name(n), currentHP(hp), maxHP(hp), currentEnergy(1), maxEnergy(5), 
      level(1), currentXP(0), xpToNextLevel(100), unallocatedStatPoints(0) {
    
    baseAttributes[STR] = str;
    baseAttributes[DEX] = dex;
    baseAttributes[CON] = con;
    baseAttributes[INT] = intel;
    baseAttributes[WIS] = wis;
    baseAttributes[CHA] = cha;

    
    loadout[WEAPON] = Equipment();
    loadout[ARMOR] = Equipment();
    loadout[RELIC] = Equipment();
}


int Entity::GetTotalStat(StatType type) const {
    int total = baseAttributes.at(type);
    
    
    for (const auto& pair : loadout) {
        auto it = pair.second.statModifiers.find(type);
        if (it != pair.second.statModifiers.end()) {
            total += it->second;
        }
    }
    
    return (total < 1) ? 1 : total;
}


void Entity::GainXP(int amount) {
    currentXP += amount;
    std::cout << "  [+] " << name << " gained " << amount << " XP! (" << currentXP << "/" << xpToNextLevel << ")\n";
    
    while (currentXP >= xpToNextLevel) {
        LevelUp();
    }
}

void Entity::LevelUp() {
    currentXP -= xpToNextLevel;
    level++;
    xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5); 
    unallocatedStatPoints += 2; 
    
    
    Heal(maxHP / 4); 

    std::cout << "\n*** LEVEL UP! " << name << " reached Level " << level << "! ***\n";
    std::cout << "*** +2 Unallocated Stat Points! ***\n";
}

void Entity::AllocateStat(StatType type) {
    if (unallocatedStatPoints > 0) {
        baseAttributes[type]++;
        unallocatedStatPoints--;
    }
}

void Entity::EquipItem(Equipment item) {
    std::cout << "\n> " << name << " equipped [" << item.name << "] in the ";
    if (item.slot == WEAPON) std::cout << "WEAPON slot.\n";
    else if (item.slot == ARMOR) std::cout << "ARMOR slot.\n";
    else std::cout << "RELIC slot.\n";
    
    loadout[item.slot] = item; 
}


void Entity::TakeDamage(int amount, DamageType type) {
    
    if (type == BLUNT || type == SLASHING) {
        int reduction = GetTotalStat(CON) / 2;
        amount -= reduction;
        if (amount < 1) amount = 1; 
    }

    currentHP -= amount;
    if (currentHP < 0) currentHP = 0;
    
    std::cout << "  [!] " << name << " takes " << amount << " damage! (HP: " << currentHP << "/" << maxHP << ")\n";
}

void Entity::Heal(int amount) {
    currentHP += amount;
    if (currentHP > maxHP) currentHP = maxHP;
    std::cout << "  [+] " << name << " recovers " << amount << " HP.\n";
}



void Entity::ApplyStatus(StatusEffect effect) {
    
    if (effect.type != CHARGED && effect.type != STEALTH && effect.type != IMMUNITY && effect.type != PROVOKED) {
        for (const auto& active : activeEffects) {
            if (active.type == IMMUNITY) {
                std::cout << "  [" << name << " is IMMUNE to " << effect.name << "!]\n";
                return; 
            }
        }
    }
    
    activeEffects.push_back(effect);
    std::cout << "  [*] " << name << " gained status: " << effect.name << "!\n";
}

void Entity::ProcessStatuses() {
    for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
        
        if (it->type == BLEED || it->type == POISON || it->type == VOID_DECAY) {
            std::cout << "  [!] " << name << " suffers " << it->potency << " damage from " << it->name << "!\n";
            TakeDamage(it->potency, TRUE_DAMAGE); 
        }

        
        it->duration--;
        if (it->duration <= 0) {
            std::cout << "  [-] " << it->name << " faded from " << name << ".\n";
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void Entity::ClearDebuffs() {
    
    for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
        
        if (it->type != CHARGED && it->type != STEALTH && it->type != IMMUNITY && it->type != PROVOKED) {
            std::cout << "  [-] Purged debuff: " << it->name << "\n";
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void Entity::ModifyBaseStat(StatType type, int amount) {
    baseAttributes[type] += amount;
    if (baseAttributes[type] < 1) baseAttributes[type] = 1; 
}

void Entity::ModifyMaxHP(int amount) {
    maxHP += amount;
    if (maxHP < 10) maxHP = 10; 
    if (currentHP > maxHP) currentHP = maxHP;
}

void Entity::ModifyMaxEnergy(int amount) {
    maxEnergy += amount;
    if (maxEnergy < 1) maxEnergy = 1;
    if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;
}




Character::Character(std::string n, std::unique_ptr<JobClass> startingJob, int hp, int str, int dex, int con, int intel, int wis, int cha)
    : Entity(n, hp, str, dex, con, intel, wis, cha), currentJob(std::move(startingJob)) {}

void Character::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    RegenerateEnergy();
    ProcessStatuses();
    if (!IsAlive()) return;

    for (const auto& status : activeEffects) {
        if (status.type == STUN) {
            std::cout << "\n> " << name << " is STUNNED and loses their turn!\n";
            return;
        }
    }

    bool validTurn = false;
    while (!validTurn) {
        std::cout << "\n[ TURN: " << name << " ] (Energy: " << currentEnergy << "/5)\n";
        
        
        std::cout << "[1] Basic Strike (0 EN) - Standard attack.\n";
        std::cout << "[2] Guard (0 EN)        - Reduce incoming damage next turn.\n";
        std::cout << "[3] Focus (0 EN)        - Regenerate 2 Energy.\n";
        
        
        const auto& skills = currentJob->GetSkills();
        for (size_t i = 0; i < skills.size(); ++i) {
            std::cout << "[" << i + 4 << "] " << skills[i].name 
                      << " (" << skills[i].energyCost << " EN) - " << skills[i].description << "\n";
        }

        std::cout << "> Select Skill (1-" << skills.size() + 3 << "): " << std::flush;
        int skillChoice;
        std::cin >> skillChoice;

        
        if (skillChoice > 3) {
            int skillIndex = skillChoice - 4;
            if (currentEnergy < skills[skillIndex].energyCost) {
                std::cout << "\n[!] Not enough Energy for that skill!\n";
                continue;
            }
        }

        

        
        std::cout << "> Select Target Enemy (Number): " << std::flush;
        int targetIdx;
        std::cin >> targetIdx;

        
        std::shared_ptr<Entity> target = Algorithms::FindTargetByIndex(enemies, targetIdx);

        if (target != nullptr) {
            std::vector<std::shared_ptr<Entity>> targetArray;
            targetArray.push_back(target);
            
            
            if (skillChoice == 1) {
                std::cout << "> " << name << " strikes " << target->GetName() << "!\n";
                target->TakeDamage(5 + GetTotalStat(STR), BLUNT);
            } else if (skillChoice == 2) {
                std::cout << "> " << name << " raises their guard!\n";
                ApplyStatus({IMMUNITY, "Guarding", 1, 0});
            } else if (skillChoice == 3) {
                std::cout << "> " << name << " focuses their mind! (+2 Energy)\n";
                RegenerateEnergy(); RegenerateEnergy();
            } 
            
            else {
                currentJob->ExecuteSkill(skillChoice - 4, allies[0], allies, targetArray); 
            }
            validTurn = true;
        } else {
            std::cout << "\n[!] Invalid target number. Try again.\n";
        }
    }
}





void Entity::SpendEnergy(int amount) {
    currentEnergy -= amount;
    if (currentEnergy < 0) currentEnergy = 0;
}

void Entity::RegenerateEnergy() {
    if (currentEnergy < maxEnergy) {
        currentEnergy++;
    }
}

bool Entity::IsAlive() const { 
    return currentHP > 0; 
}

std::string Entity::GetName() const { 
    return name; 
}

int Entity::GetHP() const { 
    return currentHP; 
}

int Entity::GetMaxHP() const { 
    return maxHP; 
}

int Entity::GetEnergy() const { 
    return currentEnergy; 
}

int Entity::GetLevel() const { 
    return level; 
}

int Entity::GetUnallocatedPoints() const { 
    return unallocatedStatPoints; 
}

void Character::EvolveJob(std::unique_ptr<JobClass> newJob) {
    std::cout << "\n*** " << name << " HAS EVOLVED! ***\n";
    currentJob = std::move(newJob); 
}