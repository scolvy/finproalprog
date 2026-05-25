#include "EventSystem.hpp"
#include <iostream>
#include <cstdlib>

void EventSystem::ResolveRandomEvent(std::shared_ptr<Entity> player, int& partyScrap) {
    
    int roll = rand() % 3;
    
    if (roll == 0) EventCaveIn(player, partyScrap);
    else if (roll == 1) EventMutatedAlchemist(player);
    else if (roll == 2) EventBlackMarket(player);
}




void EventSystem::EventCaveIn(std::shared_ptr<Entity> player, int& partyScrap) {
    std::cout << "================================================================================\n";
    std::cout << " [ EVENT: THE CAVE-IN ]\n";
    std::cout << "================================================================================\n";
    std::cout << "The ceiling of the ruins collapses! Massive debris blocks your path.\n\n";
    
    std::cout << "[1] [STR Check] Smash through the rocks with pure force.\n";
    std::cout << "[2] [DEX Check] Carefully vault over the unstable rubble.\n";
    std::cout << "> Choice: " << std::flush;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        if (player->GetTotalStat(STR) >= 15) {
            std::cout << "\n[SUCCESS] You shatter the rocks! You find hidden scrap inside! (+50 Scrap)\n";
            partyScrap += 50;
        } else {
            std::cout << "\n[FAILURE] The rocks are too heavy. You are crushed beneath them!\n";
            player->TakeDamage(25, BLUNT);
        }
    } else {
        if (player->GetTotalStat(DEX) >= 14) {
            std::cout << "\n[SUCCESS] You nimbly bypass the hazard unscathed.\n";
        } else {
            std::cout << "\n[FAILURE] The rubble shifts, catching your leg as you jump!\n";
            player->TakeDamage(20, BLUNT);
            player->ApplyStatus({BLEED, "Deep Gash", 3, 5}); 
        }
    }
}




void EventSystem::EventMutatedAlchemist(std::shared_ptr<Entity> player) {
    std::cout << "================================================================================\n";
    std::cout << " [ EVENT: MUTATED ALCHEMIST ]\n";
    std::cout << "================================================================================\n";
    std::cout << "A glowing, multi-limbed figure offers you a bubbling green vial.\n";
    std::cout << "\"Drink... evolve...\" it whispers.\n\n";
    
    std::cout << "[1] [CON Check] Drink the mysterious vial.\n";
    std::cout << "[2] Walk away safely.\n";
    std::cout << "> Choice: " << std::flush;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        if (player->GetTotalStat(CON) >= 12) {
            std::cout << "\n[SUCCESS] The liquid burns, but your muscles swell with new power!\n";
            std::cout << "*** PERMANENT GAIN: +3 STR, +3 CON ***\n";
            player->ModifyBaseStat(STR, 3);
            player->ModifyBaseStat(CON, 3);
        } else {
            std::cout << "\n[FAILURE] Your body rejects the mutagen! You violently violently vomit.\n";
            std::cout << "*** PERMANENT LOSS: -1 Max Energy ***\n";
            player->TakeDamage(15, TRUE_DAMAGE);
            player->ModifyMaxEnergy(-1);
        }
    } else {
        std::cout << "\nYou back away slowly. The creature hisses but lets you leave.\n";
    }
}




void EventSystem::EventBlackMarket(std::shared_ptr<Entity> player) {
    std::cout << "================================================================================\n";
    std::cout << " [ EVENT: THE BLACK MARKET ]\n";
    std::cout << "================================================================================\n";
    std::cout << "A shady back-alley ripperdoc offers you a 'Surgical Augment'.\n";
    std::cout << "It requires sacrificing your own flesh, but the mechanical upgrades are permanent.\n\n";
    
    std::cout << "[1] Sacrifice 15 Max HP for +4 to ANY stat.\n";
    std::cout << "[2] Refuse the surgery.\n";
    std::cout << "> Choice: " << std::flush;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        std::cout << "\nThe ripperdoc powers up a rusty saw. (-15 Max HP)\n";
        player->ModifyMaxHP(-15);
        
        std::cout << "Which system do you want augmented?\n";
        std::cout << "[1] Hydraulic Arms (+4 STR)   [4] Neural Processor (+4 INT)\n";
        std::cout << "[2] Synaptic Wires (+4 DEX)   [5] Pain Inhibitors (+4 WIS)\n";
        std::cout << "[3] Sub-dermal Plating (+4 CON)\n";
        std::cout << "> Choice: " << std::flush;
        
        int statChoice;
        std::cin >> statChoice;
        
        if (statChoice == 1) { player->ModifyBaseStat(STR, 4); std::cout << "*** +4 STR! ***\n"; }
        else if (statChoice == 2) { player->ModifyBaseStat(DEX, 4); std::cout << "*** +4 DEX! ***\n"; }
        else if (statChoice == 3) { player->ModifyBaseStat(CON, 4); std::cout << "*** +4 CON! ***\n"; }
        else if (statChoice == 4) { player->ModifyBaseStat(INTELLIGENCE, 4); std::cout << "*** +4 INT! ***\n"; }
        else if (statChoice == 5) { player->ModifyBaseStat(WIS, 4); std::cout << "*** +4 WIS! ***\n"; }
        
    } else {
        std::cout << "\nYou keep your flesh intact and walk away.\n";
    }
}