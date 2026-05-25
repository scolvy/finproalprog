#include "Jobs.hpp"
#include "Entities.hpp" // Now it's safe to include Entities for the math
#include <iostream>

// =========================================================
// 1. THE SCRAPPER
// =========================================================
Scrapper::Scrapper() {
    jobName = "Scrapper";
    skillTree.push_back({"Strike", 0, "Deals physical damage. Scales 1.5x with STR."});
    skillTree.push_back({"Guard", 1, "Applies Immunity to debuffs for 1 turn."});
}

void Scrapper::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { // Strike
        int damage = 5 + static_cast<int>(user->GetTotalStat(STR) * 1.5);
        std::cout << "> " << user->GetName() << " swings a rusted pipe!\n";
        enemies[0]->TakeDamage(damage, BLUNT); // Simplified targeting first enemy for now
    } 
    else if (skillIndex == 1) { // Guard
        std::cout << "> " << user->GetName() << " braces for impact!\n";
        user->ApplyStatus({IMMUNITY, "Guard", 1, 0});
        user->SpendEnergy(1);
    }
}

// =========================================================
// 2. THE DRIFTER
// =========================================================
Drifter::Drifter() {
    jobName = "Drifter";
    skillTree.push_back({"Shiv", 0, "Fast piercing strike. Scales 1.8x with DEX."});
    skillTree.push_back({"Smokescreen", 1, "Grants Stealth to self for 1 turn."});
}

void Drifter::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { // Shiv
        int damage = 8 + static_cast<int>(user->GetTotalStat(DEX) * 1.8);
        std::cout << "> " << user->GetName() << " lunges with a hidden blade!\n";
        enemies[0]->TakeDamage(damage, PIERCING);
    } 
    else if (skillIndex == 1) { // Smokescreen
        std::cout << "> " << user->GetName() << " drops a smoke bomb!\n";
        user->ApplyStatus({STEALTH, "Hidden", 1, 0});
        user->SpendEnergy(1);
    }
}

// =========================================================
// 3. THE SPARKMAGE
// =========================================================
Sparkmage::Sparkmage() {
    jobName = "Sparkmage";
    skillTree.push_back({"Arcane Bolt", 0, "Fires raw energy. Scales 2.0x with INT."});
    skillTree.push_back({"Static Field", 2, "Minor AoE Arcane damage to all enemies."});
}

void Sparkmage::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { // Arcane Bolt
        int damage = 10 + static_cast<int>(user->GetTotalStat(INT) * 2.0);
        std::cout << "> " << user->GetName() << " fires a crackling bolt of energy!\n";
        enemies[0]->TakeDamage(damage, ARCANE);
    } 
    else if (skillIndex == 1) { // Static Field
        std::cout << "> " << user->GetName() << " unleashes a Static Field!\n";
        int damage = 5 + static_cast<int>(user->GetTotalStat(INT) * 1.0);
        for (auto& enemy : enemies) {
            if (enemy->IsAlive()) enemy->TakeDamage(damage, ARCANE);
        }
        user->SpendEnergy(2);
    }
}

// =========================================================
// 4. THE APOTHECARY
// =========================================================
Apothecary::Apothecary() {
    jobName = "Apothecary";
    skillTree.push_back({"Mend", 1, "Heals an ally. Scales 2.0x with WIS."});
    skillTree.push_back({"Toxic Flask", 1, "Applies Poison to an enemy."});
}

void Apothecary::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { // Mend
        int healAmount = 15 + static_cast<int>(user->GetTotalStat(WIS) * 2.0);
        std::cout << "> " << user->GetName() << " administers a healing salve!\n";
        party[0]->Heal(healAmount); // Targets Vanguard by default right now
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { // Toxic Flask
        std::cout << "> " << user->GetName() << " shatters a vial of acid!\n";
        int poisonDamage = 5 + static_cast<int>(user->GetTotalStat(INT) * 0.5);
        enemies[0]->ApplyStatus({POISON, "Acid Burn", 3, poisonDamage});
        user->SpendEnergy(1);
    }
}

// =========================================================
// 5. THE ENVOY
// =========================================================
Envoy::Envoy() {
    jobName = "Envoy";
    skillTree.push_back({"Inspire", 1, "Grants +1 Energy to an ally. Scales with CHA."});
    skillTree.push_back({"Distract", 0, "Applies Blindness to an enemy."});
}

void Envoy::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { // Inspire
        std::cout << "> " << user->GetName() << " shouts tactical orders!\n";
        party[0]->RegenerateEnergy(); // Simplified to target first party member
        // High CHA makes the buff better
        if (user->GetTotalStat(CHA) > 12) {
            party[0]->ApplyStatus({CHARGED, "Inspired", 2, 0});
        }
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { // Distract
        std::cout << "> " << user->GetName() << " throws debris to distract the enemy!\n";
        // Blindness reduces enemy accuracy
        enemies[0]->ApplyStatus({BLINDNESS, "Distracted", 2, 0});
    }
}

// =========================================================
// 1. THE JUGGERNAUT (Solo Tank)
// =========================================================
Juggernaut::Juggernaut() {
    jobName = "Juggernaut";
    skillTree.push_back({"Heavy Strike", 0, "Deals physical damage. Scales 2.0x with STR."});
    skillTree.push_back({"Earthshatter", 3, "AoE STR damage. Applies Stun to all enemies."});
}

void Juggernaut::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        int damage = 10 + static_cast<int>(user->GetTotalStat(STR) * 2.0);
        std::cout << "> " << user->GetName() << " crushes the enemy with a sledgehammer!\n";
        enemies[0]->TakeDamage(damage, BLUNT);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " obliterates the ground, causing a shockwave!\n";
        int damage = 15 + static_cast<int>(user->GetTotalStat(STR) * 1.5);
        for (auto& enemy : enemies) {
            if (enemy->IsAlive()) {
                enemy->TakeDamage(damage, BLUNT);
                enemy->ApplyStatus({STUN, "Concussed", 1, 0});
            }
        }
        user->SpendEnergy(3);
    }
}

// =========================================================
// 2. THE PHANTOM (Solo Assassin)
// =========================================================
Phantom::Phantom() {
    jobName = "Phantom";
    skillTree.push_back({"Vanish", 1, "Grants Stealth for 2 turns. Next attack deals double."});
    skillTree.push_back({"Throat Slit", 2, "Massive Piercing damage. Scales 3.0x with DEX."});
}

void Phantom::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " fades into the shadows...\n";
        user->ApplyStatus({STEALTH, "Hidden", 2, 0});
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { 
        int damage = 20 + static_cast<int>(user->GetTotalStat(DEX) * 3.0);
        std::cout << "> " << user->GetName() << " strikes from the dark!\n";
        enemies[0]->TakeDamage(damage, PIERCING);
        user->SpendEnergy(2);
    }
}

// =========================================================
// 3. THE VOIDCALLER (Solo DoT Mage)
// =========================================================
Voidcaller::Voidcaller() {
    jobName = "Voidcaller";
    skillTree.push_back({"Abyssal Grasp", 1, "Applies Void Decay (True Dmg DoT) to an enemy."});
    skillTree.push_back({"Event Horizon", 4, "Consumes all Void Decay on the field for an AoE Nuke."});
}

void Voidcaller::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " conjures dark matter!\n";
        int dotDamage = 10 + static_cast<int>(user->GetTotalStat(INT) * 1.0);
        enemies[0]->ApplyStatus({VOID_DECAY, "Decaying", 3, dotDamage});
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " rips spacetime apart!\n";
        int baseDamage = 30 + static_cast<int>(user->GetTotalStat(INT) * 2.5);
        for (auto& enemy : enemies) {
            if (enemy->IsAlive()) enemy->TakeDamage(baseDamage, ARCANE);
        }
        user->SpendEnergy(4);
    }
}

// =========================================================
// 4. THE FLESH-MENDER (Solo Survivalist)
// =========================================================
FleshMender::FleshMender() {
    jobName = "Flesh-Mender";
    skillTree.push_back({"Revitalize", 1, "Heavy self-heal. Scales with WIS."});
    skillTree.push_back({"Purge", 1, "Clears all negative status effects from self."});
}

void FleshMender::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        int healAmount = 25 + static_cast<int>(user->GetTotalStat(WIS) * 2.5);
        std::cout << "> " << user->GetName() << " reconstructs their own tissue!\n";
        user->Heal(healAmount);
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " purges their bloodstream!\n";
        // Logic to clear debuffs will be handled in the Entity class
        std::cout << "  [+] All debuffs cleared.\n";
        user->SpendEnergy(1);
    }
}

// =========================================================
// 5. THE COMMANDER (Solo Tactician)
// =========================================================
Commander::Commander() {
    jobName = "Commander";
    skillTree.push_back({"Hold the Line!", 2, "Grants temporary HP and Immunity. Scales with CHA."});
    skillTree.push_back({"Suppressing Fire", 1, "Reduces all enemies' damage output (Fear)."});
}

void Commander::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " bolsters their own resolve!\n";
        int tempHP = 15 + static_cast<int>(user->GetTotalStat(CHA) * 2.0);
        user->Heal(tempHP); // Acts as a burst heal in a solo run
        user->ApplyStatus({IMMUNITY, "Iron Will", 2, 0});
        user->SpendEnergy(2);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " lays down covering fire!\n";
        for (auto& enemy : enemies) {
            if (enemy->IsAlive()) {
                enemy->ApplyStatus({FEAR, "Suppressed", 2, 0});
            }
        }
        user->SpendEnergy(1);
    }
}

// =========================================================
// 6. THE BLOOD-BRAWLER (Solo Berserker)
// =========================================================
BloodBrawler::BloodBrawler() {
    jobName = "Blood-Brawler";
    skillTree.push_back({"Bloodletting", 0, "Take 15% Max HP as True Damage to instantly gain 2 Energy."});
    skillTree.push_back({"Carnage", 3, "Strikes 4 times. Damage scales with STR and MISSING Health."});
}

void BloodBrawler::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        int hpCost = static_cast<int>(user->GetMaxHP() * 0.15);
        std::cout << "> " << user->GetName() << " slashes their own arm! (-" << hpCost << " HP)\n";
        user->TakeDamage(hpCost, TRUE_DAMAGE); // True damage ignores armor
        
        // Custom energy gain bypassing the normal cap for a burst turn
        for (int i = 0; i < 2; i++) user->RegenerateEnergy();
    } 
    else if (skillIndex == 1) { 
        int missingHP = user->GetMaxHP() - user->GetHP();
        // Base damage + STR + 50% of missing HP
        int damagePerHit = 5 + static_cast<int>(user->GetTotalStat(STR) * 1.5) + static_cast<int>(missingHP * 0.5);
        
        std::cout << "> " << user->GetName() << " enters a blood-fueled frenzy!\n";
        for (int i = 0; i < 4; i++) {
            if (enemies[0]->IsAlive()) {
                std::cout << "  Hit " << (i+1) << "!\n";
                enemies[0]->TakeDamage(damagePerHit, BLUNT);
            }
        }
        user->SpendEnergy(3);
    }
}

// =========================================================
// 7. THE GUNSLINGER (Solo Ranged DPS)
// =========================================================
Gunslinger::Gunslinger() {
    jobName = "Gunslinger";
    skillTree.push_back({"Fan the Hammer", 0, "Expends ALL current Energy. Fires 1 shot per Energy spent."});
    skillTree.push_back({"Kneecap", 1, "Applies SLOW to an enemy, ruining their turn speed."});
}

void Gunslinger::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        int shots = user->GetEnergy();
        if (shots == 0) {
            std::cout << "> " << user->GetName() << "'s gun clicks empty. No energy!\n";
            return;
        }
        
        int damagePerShot = 15 + static_cast<int>(user->GetTotalStat(DEX) * 2.0);
        std::cout << "> " << user->GetName() << " fans the hammer! Firing " << shots << " shots!\n";
        
        for (int i = 0; i < shots; i++) {
            if (enemies[0]->IsAlive()) {
                enemies[0]->TakeDamage(damagePerShot, PIERCING);
            }
        }
        user->SpendEnergy(shots); // Dumps all energy
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " takes a precise shot at the enemy's legs!\n";
        int damage = 10 + user->GetTotalStat(DEX);
        enemies[0]->TakeDamage(damage, PIERCING);
        enemies[0]->ApplyStatus({SLOW, "Crippled", 2, 0});
        user->SpendEnergy(1);
    }
}

// =========================================================
// 8. THE STORMWEAVER (Solo Crowd Control)
// =========================================================
Stormweaver::Stormweaver() {
    jobName = "Stormweaver";
    skillTree.push_back({"Chain Lightning", 3, "Bounces to 3 enemies, reducing their attack damage."});
    skillTree.push_back({"Overload", 0, "Take minor Arcane damage to generate 2 Energy."});
}

void Stormweaver::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " hurls a bolt of chain lightning!\n";
        int damage = 20 + static_cast<int>(user->GetTotalStat(INT) * 2.0);
        int bounceCount = 0;
        
        for (auto& enemy : enemies) {
            if (enemy->IsAlive() && bounceCount < 3) {
                std::cout << "  ZAP! Lightning strikes " << enemy->GetName() << "!\n";
                enemy->TakeDamage(damage, ARCANE);
                enemy->ApplyStatus({FEAR, "Shocked", 2, 0}); // Represents reduced attack damage
                bounceCount++;
            }
        }
        user->SpendEnergy(3);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " overloads their own nervous system!\n";
        user->TakeDamage(10, ARCANE);
        for (int i = 0; i < 2; i++) user->RegenerateEnergy();
    }
}

// =========================================================
// 9. THE PLAGUE DOCTOR (Solo Debuffer)
// =========================================================
PlagueDoctor::PlagueDoctor() {
    jobName = "Plague Doctor";
    skillTree.push_back({"Noxious Cloud", 2, "Applies Poison and Blindness to ALL enemies."});
    skillTree.push_back({"Transfusion", 2, "Drains HP from the enemy and heals you."});
}

void PlagueDoctor::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " unleashes a toxic gas grenade!\n";
        int poisonDamage = 10 + static_cast<int>(user->GetTotalStat(INT) * 1.5);
        
        for (auto& enemy : enemies) {
            if (enemy->IsAlive()) {
                enemy->ApplyStatus({POISON, "Toxic Rot", 3, poisonDamage});
                enemy->ApplyStatus({BLINDNESS, "Blinded", 2, 0});
            }
        }
        user->SpendEnergy(2);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " uses leeches to siphon life force!\n";
        int drainAmount = 20 + static_cast<int>(user->GetTotalStat(WIS) * 2.0);
        
        enemies[0]->TakeDamage(drainAmount, TRUE_DAMAGE);
        user->Heal(drainAmount); // Lifesteal mechanic
        user->SpendEnergy(2);
    }
}

// =========================================================
// 10. THE CHARLATAN (Solo Manipulator)
// =========================================================
Charlatan::Charlatan() {
    jobName = "Charlatan";
    skillTree.push_back({"Bribery", 1, "Forces a non-boss enemy to flee combat."});
    skillTree.push_back({"Hallucination", 2, "Applies Mind-Control. Enemy attacks its allies."});
}

void Charlatan::ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) {
    if (skillIndex == 0) { 
        std::cout << "> " << user->GetName() << " tosses a pouch of fake scrap at the enemy!\n";
        
        if (enemies[0]->IsAlive()) {
            std::cout << "  " << enemies[0]->GetName() << " scrambles for the scrap and flees the battle!\n";
            enemies[0]->TakeDamage(9999, TRUE_DAMAGE);
        }
        user->SpendEnergy(1);
    } 
    else if (skillIndex == 1) { 
        std::cout << "> " << user->GetName() << " releases a hallucinogenic dart!\n";
        enemies[0]->ApplyStatus({MIND_CONTROL, "Hallucinating", 1, 0});
        user->SpendEnergy(2);
    }
}