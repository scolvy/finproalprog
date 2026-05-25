#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include<vector>
#include<memory>

// =========================================================
// 1. D&D ATTRIBUTE SYSTEM (From GDD Section 3)
// =========================================================
enum StatType { 
    STR, // Scales heavy physical, carrying capacity, physical checks
    DEX, // Scales precision damage, Initiative, Dodge, trap bypass
    CON, // Scales Max HP, physical status resistance (Bleed, Stun)
    INT, // Scales Arcane damage, Max Energy, ancient text deciphering
    WIS, // Scales Healing, Shielding, mental status resistance
    CHA  // Scales Buff/Debuff duration, merchant discounts
};

// =========================================================
// 2. DAMAGE TYPES (From GDD Section 3)
// =========================================================
enum DamageType { 
    BLUNT,      // STR based
    SLASHING,   // STR based
    PIERCING,   // DEX based
    ARCANE,     // INT based
    TRUE_DAMAGE // Ignores armor (e.g., Void Decay, Bleed)
};

// =========================================================
// 3. STATUS EFFECTS (From GDD Section 4 & 7)
// =========================================================
enum StatusType { 
    NONE,
    // Physical Debuffs (Resisted by CON)
    BLEED,       // True damage over time
    SHATTER,     // Armor reduction
    STUN,        // Skips turn
    POISON,      // Damage over time, reduces healing
    SLOW,        // Reduces DEX/Initiative to 0
    
    // Mental/Arcane Debuffs (Resisted by WIS)
    FEAR,        // Reduces damage output
    BLINDNESS,   // Attacks have a high chance to miss
    MIND_CONTROL,// Attacks an ally
    VOID_DECAY,  // Stacking Arcane DoT that ignores armor
    
    // Buffs & Utility (Duration scales with CHA)
    PROVOKED,    // Forces target selection (Aggro control)
    CHARGED,     // Boosts Energy generation
    STEALTH,     // Untargetable for N turns
    IMMUNITY     // Ignores incoming debuffs
};

// =========================================================
// 4. DATA STRUCTURES
// =========================================================
struct StatusEffect {
    StatusType type;
    std::string name;
    int duration; // Measured in turns
    int potency;  // How strong the effect is (e.g., Bleed damage)
};

struct Skill {
    std::string name;
    int energyCost;
    std::string description;
};

// Forward declaration for the Job Component System
class Entity; 

// =========================================================
// 5. ABSTRACT JOB CLASS (The Strategy/Component Pattern)
// =========================================================
// This ensures we can hot-swap a Character's class mid-run 
// when they evolve at an Event Node.
class JobClass {
protected:
    std::string jobName;
    std::vector<Skill> skillTree;

public:
    virtual ~JobClass() = default;
    
    std::string GetJobName() const { return jobName; }
    const std::vector<Skill>& GetSkills() const { return skillTree; }

    // Pure virtual execution: Each specific class will define its own math here
    virtual void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) = 0;
};

#endif