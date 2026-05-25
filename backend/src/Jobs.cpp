#include "Jobs.hpp"
#include "Entities.hpp"
#include <iostream>

Scrapper::Scrapper() {
    jobName = "Scrapper";
    skillTree = {
        {"Strike", 0, "A standard heavy-duty smash using a rusted pipe. Deals 5 base physical damage plus 150% of the user's Strength stat. Pierces basic armor."},
        {"Guard", 1, "The Scrapper assumes a defensive stance, applying an Immunity status effect for 1 turn that prevents all incoming debuffs and crowd control."},
        {"Provoke", 5, "The Scrapper shouts a challenge, taunting all enemies on the field. Inflicts Fear and forces them to target the Scrapper for 2 turns."}
    };
}
void Scrapper::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) e[0]->TakeDamage(5 + (user->GetTotalStat(STR) * 1.5), BLUNT);
    else if (sIdx == 1) { user->ApplyStatus({IMMUNITY, "Guard", 1, 0}); user->SpendEnergy(1); }
    else if (sIdx == 2) { for(auto& enemy : e) enemy->ApplyStatus({FEAR, "Taunted", 2, 0}); user->SpendEnergy(5); }
}

Drifter::Drifter() {
    jobName = "Drifter";
    skillTree = {
        {"Shiv", 0, "A rapid, precise piercing strike targeting enemy weaknesses. Deals 8 base damage plus 180% of the user's Dexterity stat."},
        {"Smokescreen", 1, "The Drifter vanishes behind a chemical cloud, granting themselves the Stealth status for 1 turn, rendering them untargetable by direct attacks."},
        {"Assassinate", 4, "A lethal execution move aimed at a single target. Deals 40 base damage plus 200% of the user's Dexterity stat. Ignores physical resistance."}
    };
}
void Drifter::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) e[0]->TakeDamage(8 + (user->GetTotalStat(DEX) * 1.8), PIERCING);
    else if (sIdx == 1) { user->ApplyStatus({STEALTH, "Hidden", 1, 0}); user->SpendEnergy(1); }
    else if (sIdx == 2) { e[0]->TakeDamage(40 + (user->GetTotalStat(DEX) * 2), PIERCING); user->SpendEnergy(4); }
}

Sparkmage::Sparkmage() {
    jobName = "Sparkmage";
    skillTree = {
        {"Arcane Bolt", 0, "Channels raw arcane energy into a concentrated projectile. Deals 10 base damage plus 200% of the user's Intelligence stat."},
        {"Static Field", 2, "Releases a volatile wave of electrical discharge. Deals 5 base damage plus 100% of Intelligence to all active enemies simultaneously."},
        {"Meteor", 6, "Summons a devastating arcane meteor from the upper atmosphere. Deals 50 flat arcane damage to the entire enemy formation."}
    };
}
void Sparkmage::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) e[0]->TakeDamage(10 + (user->GetTotalStat(INTELLIGENCE) * 2.0), ARCANE);
    else if (sIdx == 1) { for (auto& enemy : e) enemy->TakeDamage(5 + user->GetTotalStat(INTELLIGENCE), ARCANE); user->SpendEnergy(2); }
    else if (sIdx == 2) { for (auto& enemy : e) enemy->TakeDamage(50, ARCANE); user->SpendEnergy(6); }
}

Apothecary::Apothecary() {
    jobName = "Apothecary";
    skillTree = {
        {"Mend", 1, "Applies a potent biological healing salve to an ally. Restores 15 HP plus 200% of the user's Wisdom stat."},
        {"Toxic Flask", 1, "Hurls a pressurized flask of caustic acid. Applies Poison to the target for 3 turns, dealing 5 base damage per tick."},
        {"Panacea", 4, "A master-crafted restorative cocktail that stabilizes the entire party. Restores 50 HP to every ally simultaneously."}
    };
}
void Apothecary::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) {
        if (IsValidTarget(0, p)) { 
            p[0]->Heal(15 + (user->GetTotalStat(WIS) * 2.0)); 
            user->SpendEnergy(1);
        }
    } 
    else if (sIdx == 1) { 
        if (IsValidTarget(0, e)) { 
            e[0]->ApplyStatus({POISON, "Acid", 3, 5}); 
            user->SpendEnergy(1); 
        }
    } 
    else if (sIdx == 2) { 
        for(auto& ally : p) if(ally->IsAlive()) ally->Heal(50); 
        user->SpendEnergy(4); 
    }
}

Envoy::Envoy() {
    jobName = "Envoy";
    skillTree = {
        {"Inspire", 1, "Delivers a morale-boosting command to an ally, immediately granting them 1 Energy point to perform an extra action."},
        {"Distract", 0, "Uses flares and sound to confuse the enemy. Applies the Blindness status effect for 2 turns, reducing enemy accuracy."},
        {"Rally", 5, "Unleashes an empowering war-cry that echoes through the party. All allies immediately regenerate 1 Energy point."}
    };
}
void Envoy::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) {
        if (IsValidTarget(0, p)) { 
            p[0]->RegenerateEnergy(); 
            user->SpendEnergy(1);
        }
    } 
    else if (sIdx == 1) { 
        if (IsValidTarget(0, e)) { 
            e[0]->ApplyStatus({BLINDNESS, "Distracted", 2, 0}); 
        }
    } 
    else if (sIdx == 2) { 
        for(auto& ally : p) if(ally->IsAlive()) ally->RegenerateEnergy();
        user->SpendEnergy(5);
    }
}

Juggernaut::Juggernaut() {
    jobName = "Juggernaut";
    skillTree = {
        {"Heavy Strike", 0, "A crushing blow with a massive sledgehammer. Deals 10 base damage plus 200% of the user's Strength stat."},
        {"Earthshatter", 3, "Slams the ground to create a localized seismic event. Deals 15 flat damage and applies Stun to all enemies for 1 turn."},
        {"Siege Breaker", 6, "A momentum-driven strike capable of shattering barricades. Deals 80 base damage to one target. Guaranteed to hit."}
    };
}
void Juggernaut::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) e[0]->TakeDamage(10 + (user->GetTotalStat(STR) * 2), BLUNT);
    else if (sIdx == 1) { for (auto& enemy : e) { enemy->TakeDamage(15, BLUNT); enemy->ApplyStatus({STUN, "Dazed", 1, 0}); } user->SpendEnergy(3); }
    else if (sIdx == 2) { e[0]->TakeDamage(80, BLUNT); user->SpendEnergy(6); }
}

Phantom::Phantom() {
    jobName = "Phantom";
    skillTree = {
        {"Vanish", 1, "The Phantom slips between physical planes, granting Stealth for 2 turns and immunity to single-target attacks."},
        {"Throat Slit", 2, "An surgical strike to the windpipe. Deals 20 base damage plus 300% of the user's Dexterity stat."},
        {"Death Mark", 5, "Places an ethereal mark on the enemy. Targets suffer from a lingering Poison effect, dealing 20 damage per turn for 3 turns."}
    };
}
void Phantom::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { user->ApplyStatus({STEALTH, "Hidden", 2, 0}); user->SpendEnergy(1); }
    else if (sIdx == 1) { e[0]->TakeDamage(20 + (user->GetTotalStat(DEX) * 3), PIERCING); user->SpendEnergy(2); }
    else if (sIdx == 2) { e[0]->ApplyStatus({POISON, "Marked", 3, 20}); user->SpendEnergy(5); }
}

Voidcaller::Voidcaller() {
    jobName = "Voidcaller";
    skillTree = {
        {"Abyssal Grasp", 1, "Reaches into the void to manifest dark energy. Applies Void Decay, dealing 10 true damage per turn for 3 turns."},
        {"Event Horizon", 4, "Distorts gravity in an area. Deals 30 flat Arcane damage to all enemies, bypassing conventional armor."},
        {"Singularity", 7, "Collapses a local reality point. Deals 100 flat Arcane damage to all enemies, potentially wiping out entire groups."}
    };
}
void Voidcaller::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { e[0]->ApplyStatus({VOID_DECAY, "Decaying", 3, 10}); user->SpendEnergy(1); }
    else if (sIdx == 1) { for (auto& enemy : e) enemy->TakeDamage(30, ARCANE); user->SpendEnergy(4); }
    else if (sIdx == 2) { for (auto& enemy : e) enemy->TakeDamage(100, ARCANE); user->SpendEnergy(7); }
}

FleshMender::FleshMender() {
    jobName = "Flesh-Mender";
    skillTree = {
        {"Revitalize", 1, "Self-sutures wounds and stimulates rapid cellular regeneration. Restores 25 base HP plus 250% of the user's Wisdom stat."},
        {"Purge", 1, "Uses chemical agents to cleanse the user's bloodstream, removing all accumulated negative status effects."},
        {"Resurrection", 6, "Channels forbidden energy into the body of a fallen ally. Restores the target with 1 HP if they are currently incapacitated."}
    };
}
void FleshMender::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { user->Heal(25 + (user->GetTotalStat(WIS) * 2.5)); user->SpendEnergy(1); }
    else if (sIdx == 1) { user->SpendEnergy(1); }
    else if (sIdx == 2) { for(auto& ally : p) if(!ally->IsAlive()) ally->Heal(1); user->SpendEnergy(6); }
}

Commander::Commander() {
    jobName = "Commander";
    skillTree = {
        {"Hold the Line!", 2, "Inspires the user with raw charisma. Grants 15 base temp HP plus 200% of Charisma and temporary Immunity."},
        {"Suppressing Fire", 1, "Coordinates an area-wide barrage. Applies the Fear debuff to all enemies, reducing their offensive output for 2 turns."},
        {"Total War", 5, "Orders a full-scale tactical advance. Grants all allies the Hasted status, increasing their turn priority for 3 turns."}
    };
}
void Commander::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { user->Heal(15 + (user->GetTotalStat(CHA) * 2)); user->ApplyStatus({IMMUNITY, "Iron Will", 2, 0}); user->SpendEnergy(2); }
    else if (sIdx == 1) { for (auto& enemy : e) enemy->ApplyStatus({FEAR, "Suppressed", 2, 0}); user->SpendEnergy(1); }
    else if (sIdx == 2) { for(auto& ally : p) ally->ApplyStatus({CHARGED, "Hasted", 3, 0}); user->SpendEnergy(5); }
}

BloodBrawler::BloodBrawler() {
    jobName = "Blood-Brawler";
    skillTree = {
        {"Bloodletting", 0, "Violently cuts self to siphon power. Takes 15% Max HP damage but immediately gains 2 Energy points."},
        {"Carnage", 3, "A ferocious flurry of 4 strikes in rapid succession. Each strike deals 5 base damage plus 100% of Strength."},
        {"Blood God", 6, "Communes with the primal forces of vitality. Restores 50% of the user's maximum HP instantly."}
    };
}
void BloodBrawler::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { user->TakeDamage(user->GetMaxHP() * 0.15, TRUE_DAMAGE); user->RegenerateEnergy(); user->RegenerateEnergy(); }
    else if (sIdx == 1) { for(int i=0; i<4; i++) e[0]->TakeDamage(5 + user->GetTotalStat(STR), BLUNT); user->SpendEnergy(3); }
    else if (sIdx == 2) { user->Heal(user->GetMaxHP() * 0.5); user->SpendEnergy(6); }
}

Gunslinger::Gunslinger() {
    jobName = "Gunslinger";
    skillTree = {
        {"Fan the Hammer", 0, "Unloads the magazine in a frantic burst. Fires shots equal to current Energy spent, dealing 15 plus 100% DEX per shot."},
        {"Kneecap", 1, "Targets the enemy's legs to disrupt mobility. Deals 10 piercing damage and applies the Crippled Slow debuff for 2 turns."},
        {"Bullet Hell", 5, "Sprays the entire enemy formation with suppressing fire. Deals 40 flat piercing damage to every active enemy."}
    };
}
void Gunslinger::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { int en = user->GetEnergy(); for(int i=0; i<en; i++) e[0]->TakeDamage(15 + user->GetTotalStat(DEX), PIERCING); user->SpendEnergy(en); }
    else if (sIdx == 1) { e[0]->TakeDamage(10, PIERCING); e[0]->ApplyStatus({SLOW, "Crippled", 2, 0}); user->SpendEnergy(1); }
    else if (sIdx == 2) { for(auto& enemy : e) enemy->TakeDamage(40, PIERCING); user->SpendEnergy(5); }
}

Stormweaver::Stormweaver() {
    jobName = "Stormweaver";
    skillTree = {
        {"Chain Lightning", 3, "Calls a bolt that strikes 3 enemies in sequence. Each strike deals 20 Arcane damage."},
        {"Overload", 0, "Forces electrical energy through their own nervous system. Takes 10 Arcane damage but generates 2 Energy."},
        {"Stormcaller", 7, "Summons a localized lightning storm. Deals 50 Arcane damage and applies Stun to all enemies for 2 turns."}
    };
}
void Stormweaver::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { for(int i=0; i<3; i++) e[i]->TakeDamage(20, ARCANE); user->SpendEnergy(3); }
    else if (sIdx == 1) { user->TakeDamage(10, ARCANE); user->RegenerateEnergy(); user->RegenerateEnergy(); }
    else if (sIdx == 2) { for(auto& enemy : e) { enemy->TakeDamage(50, ARCANE); enemy->ApplyStatus({STUN, "Shocked", 2, 0}); } user->SpendEnergy(7); }
}

PlagueDoctor::PlagueDoctor() {
    jobName = "Plague Doctor";
    skillTree = {
        {"Noxious Cloud", 2, "Detonates a grenade of toxic sludge. Applies Poison and Blindness to all enemies for 3 turns."},
        {"Transfusion", 2, "Uses bio-leeches to siphon life. Deals 20 true damage to one target and heals the user for 20 HP."},
        {"Pandemic", 5, "Unleashes a bio-weapon. Applies a lethal Plague status to all enemies, dealing 20 damage per turn for 5 turns."}
    };
}
void PlagueDoctor::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) { for(auto& enemy : e) { enemy->ApplyStatus({POISON, "Toxic", 3, 10}); enemy->ApplyStatus({BLINDNESS, "Blinded", 2, 0}); } user->SpendEnergy(2); }
    else if (sIdx == 1) { e[0]->TakeDamage(20, TRUE_DAMAGE); user->Heal(20); user->SpendEnergy(2); }
    else if (sIdx == 2) { for(auto& enemy : e) enemy->ApplyStatus({POISON, "Plague", 5, 20}); user->SpendEnergy(5); }
}

Charlatan::Charlatan() {
    jobName = "Charlatan";
    skillTree = {
        {"Bribery", 1, "Distributes counterfeit currency to baffle an enemy, causing them to flee the battle immediately."},
        {"Hallucination", 2, "Shoots a specialized chemical dart to scramble cognitive function. Applies Mind-Control to an enemy for 1 turn."},
        {"Jackpot", 4, "Celebrates with a theatrical display. Regenerates 1 Energy point for every member of the active party."}
    };
}
void Charlatan::ExecuteSkill(int sIdx, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& p, std::vector<std::shared_ptr<Entity>>& e) {
    if (sIdx == 0) {
        if (IsValidTarget(0, e)) { 
            e[0]->TakeDamage(9999, TRUE_DAMAGE); 
            user->SpendEnergy(1);
        }
    } 
    else if (sIdx == 1) { 
        if (IsValidTarget(0, e)) { 
            e[0]->ApplyStatus({MIND_CONTROL, "Hallucinating", 1, 0}); 
            user->SpendEnergy(2);
        }
    } 
    else if (sIdx == 2) { 
        for(auto& ally : p) if(ally->IsAlive()) ally->RegenerateEnergy();
        user->SpendEnergy(4);
    }
}