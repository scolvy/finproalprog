#include "Entities.hpp"
#include "Algorithms.hpp"


Entity::Entity(std::string n, int hp, int str, int dex, int con, int intel, int wis, int cha) 
    : name(n), currentHP(hp), maxHP(hp), currentEnergy(1), maxEnergy(5), 
      level(1), currentXP(0), xpToNextLevel(100), unallocatedStatPoints(0) {
    
    baseAttributes[STR] = str;
    baseAttributes[DEX] = dex;
    baseAttributes[CON] = con;
    baseAttributes[INTELLIGENCE] = intel;
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




Character::Character(std::string n, std::unique_ptr<JobClass> startingJob, int hp, int str, int dex, int con, int intel, int wis, int cha):
    Entity(n, hp, str, dex, con, intel, wis, cha), currentJob(std::move(startingJob)) {}

std::string Character::PromptLocalTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    bool validTurn = false;
    int choice = 0;
    int targetIdx = 0;

    while (!validTurn) {
        std::cout << "\n======================================================================\n";
        std::cout << " [ TURN: " << name << " | CLASS: " << currentJob->GetJobName() << " ] (Energy: " << currentEnergy << "/" << maxEnergy << ")\n";
        std::cout << "----------------------------------------------------------------------\n";
        std::cout << "  [1] Basic Strike (0 EN) | [2] Block (0 EN) | [3] Focus (0 EN)\n\n";
        
        
        const auto& skills = currentJob->GetSkills();
        for (size_t i = 0; i < skills.size(); ++i) {
            std::cout << "  [" << i + 4 << "] " << skills[i].name << " (" << skills[i].energyCost << " EN)\n";
            std::cout << "      > " << skills[i].description << "\n";
        }
        std::cout << "======================================================================\n";
        std::cout << "  > Select Skill: " << std::flush;
        std::cin >> choice;


        if (choice < 1 || choice > (skills.size() + 3)) {
            std::cout << "\n[!] Invalid selection.\n";
            continue;
        }

        std::cout << "> Select Target Enemy (Number): " << std::flush;
        std::cin >> targetIdx;

        if (targetIdx < 1 || targetIdx > enemies.size() || !enemies[targetIdx - 1]->IsAlive()) {
            std::cout << "\n[!] Invalid target.\n";
            continue;
        }

        if (choice > 3 && currentEnergy < skills[choice - 4].energyCost) {
            std::cout << "\n[!] Not enough Energy!\n";
            continue;
        }
        validTurn = true;
    }
    return std::to_string(choice) + "|" + std::to_string(targetIdx);
}

void Character::ExecuteNetworkTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies, int choice, int targetIdx) {
    RegenerateEnergy();
    ProcessStatuses();  
    if (!IsAlive()) return;

    for (const auto& status : activeEffects) {
        if (status.type == STUN) {
            std::cout << "\n> " << name << " is STUNNED and loses their turn!\n";
            return;
        }
    }

    std::shared_ptr<Entity> target = enemies[targetIdx - 1];

    if (choice <= 3) {
        switch(choice) {
            case 1: 
                std::cout << "> " << name << " executes a standard strike!\n";
                target->TakeDamage(5 + (GetTotalStat(STR) / 2), BLUNT);
                break;
            case 2: 
                std::cout << "> " << name << " braces for impact!\n";
                ApplyStatus({IMMUNITY, "Blocking", 1, 0});
                break;
            case 3: 
                std::cout << "> " << name << " channels energy! (+2 EN)\n";
                RegenerateEnergy(); RegenerateEnergy();
                break;
        }
    } else {
        std::vector<std::shared_ptr<Entity>> targetVector = {target};
        currentJob->ExecuteSkill(choice - 4, allies[0], allies, targetVector);
    }
}

void Character::TakeTurn(std::vector<std::shared_ptr<Entity>>& allies, std::vector<std::shared_ptr<Entity>>& enemies) {
    std::string action = PromptLocalTurn(allies, enemies);
    std::vector<std::string> tokens = NetworkUtils::Split(action, '|');
    ExecuteNetworkTurn(allies, enemies, std::stoi(tokens[0]), std::stoi(tokens[1]));
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

void Entity::SetMaxHP(int amount) {
    maxHP = amount;
}

JobClass* Character::GetCurrentJob() const {
    return currentJob.get();
}