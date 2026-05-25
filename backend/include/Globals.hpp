#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include<vector>
#include<memory>




enum StatType { 
    STR, 
    DEX, 
    CON, 
    INTELLIGENCE, 
    WIS, 
    CHA  
};




enum DamageType { 
    BLUNT,      
    SLASHING,   
    PIERCING,   
    ARCANE,     
    TRUE_DAMAGE 
};




enum StatusType { 
    NONE,
    
    BLEED,       
    SHATTER,     
    STUN,        
    POISON,      
    SLOW,        
    
    
    FEAR,        
    BLINDNESS,   
    MIND_CONTROL,
    VOID_DECAY,  
    
    
    PROVOKED,    
    CHARGED,     
    STEALTH,     
    IMMUNITY     
};




struct StatusEffect {
    StatusType type;
    std::string name;
    int duration; 
    int potency;  
};

enum SkillType {
    SKILL_DAMAGE,
    SKILL_HEALING,
    SKILL_DEBUFF,
    SKILL_BUFF,
    SKILL_UTILITY
};

struct Skill {
    std::string name;
    int energyCost;
    std::string description;
    SkillType type;
};


class Entity; 

class JobClass {
protected:
    std::string jobName;
    std::vector<Skill> skillTree;

public:
    virtual ~JobClass() = default;
    
    std::string GetJobName() const { return jobName; }
    const std::vector<Skill>& GetSkills() const { return skillTree; }

    
    virtual void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) = 0;
};

#endif