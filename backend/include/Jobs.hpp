#ifndef JOBS_HPP
#define JOBS_HPP

#include <string>
#include <vector>    
#include <memory>
#include "Globals.hpp"

// Forward declaration to avoid circular dependencies
class Entity; 

// =========================================================
// THE SCRAPPER (Frontline Melee)
// Primary Stats: STR, CON
// =========================================================
class Scrapper : public JobClass {
public:
    Scrapper();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE DRIFTER (Agile DPS)
// Primary Stats: DEX, INT
// =========================================================
class Drifter : public JobClass {
public:
    Drifter();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE SPARKMAGE (Arcane Artillery)
// Primary Stats: INT, WIS
// =========================================================
class Sparkmage : public JobClass {
public:
    Sparkmage();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE APOTHECARY (Support & Survival)
// Primary Stats: WIS, CON
// =========================================================
class Apothecary : public JobClass {
public:
    Apothecary();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE ENVOY (Control & Synergy)
// Primary Stats: CHA, DEX
// =========================================================
class Envoy : public JobClass {
public:
    Envoy();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE JUGGERNAUT (Scrapper Evolution A)
// Focus: Extreme Survivability and Stuns
// =========================================================
class Juggernaut : public JobClass {
public:
    Juggernaut();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE PHANTOM (Drifter Evolution A)
// Focus: Stealth and Execution
// =========================================================
class Phantom : public JobClass {
public:
    Phantom();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE VOIDCALLER (Sparkmage Evolution A)
// Focus: Armor-ignoring Damage Over Time (DoT)
// =========================================================
class Voidcaller : public JobClass {
public:
    Voidcaller();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE FLESH-MENDER (Apothecary Evolution A)
// Focus: Unkillable Self-Sustain
// =========================================================
class FleshMender : public JobClass {
public:
    FleshMender();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE COMMANDER (Envoy Evolution A)
// Focus: Action Economy & Temporary Shields
// =========================================================
class Commander : public JobClass {
public:
    Commander();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE BLOOD-BRAWLER (Scrapper Evolution B)
// Focus: High-Risk Burst Damage & HP Sacrifice
// =========================================================
class BloodBrawler : public JobClass {
public:
    BloodBrawler();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE GUNSLINGER (Drifter Evolution B)
// Focus: High Initiative & Energy Dumping
// =========================================================
class Gunslinger : public JobClass {
public:
    Gunslinger();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE STORMWEAVER (Sparkmage Evolution B)
// Focus: Multi-Target Crowd Control
// =========================================================
class Stormweaver : public JobClass {
public:
    Stormweaver();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE PLAGUE DOCTOR (Apothecary Evolution B)
// Focus: Extreme Debuffs & Lifesteal
// =========================================================
class PlagueDoctor : public JobClass {
public:
    PlagueDoctor();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

// =========================================================
// THE CHARLATAN (Envoy Evolution B)
// Focus: Mind Manipulation & Bypassing Combat
// =========================================================
class Charlatan : public JobClass {
public:
    Charlatan();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};
#endif