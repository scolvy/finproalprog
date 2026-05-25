#include "Algorithms.hpp"

// =========================================================
// 1. PENGURUTAN (QUICK SORT)
// Requirement: Implement Sorting manually.
// Usage: Sorts the combined party and enemies array by DEX.
// =========================================================
int Algorithms::Partition(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high) {
    // Pivot is the DEX stat of the last element
    int pivotValue = combatants[high]->GetTotalStat(DEX); 
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // DESCENDING order (Highest DEX goes first)
        if (combatants[j]->GetTotalStat(DEX) >= pivotValue) {
            i++;
            // Manual Swap
            std::shared_ptr<Entity> temp = combatants[i];
            combatants[i] = combatants[j];
            combatants[j] = temp;
        }
    }
    // Swap pivot to correct position
    std::shared_ptr<Entity> temp = combatants[i + 1];
    combatants[i + 1] = combatants[high];
    combatants[high] = temp;

    return i + 1;
}

void Algorithms::QuickSortInitiative(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high) {
    if (low < high) {
        int partitionIndex = Partition(combatants, low, high);
        
        QuickSortInitiative(combatants, low, partitionIndex - 1);
        QuickSortInitiative(combatants, partitionIndex + 1, high);
    }
}

// =========================================================
// 2. PENCARIAN (LINEAR SEARCH)
// Requirement: Implement Searching manually.
// Usage: Used when the player types an enemy's name to attack.
// =========================================================
// =========================================================
// 2. PENCARIAN (LINEAR SEARCH) - TARGET BY NUMBER
// =========================================================
std::shared_ptr<Entity> Algorithms::FindTargetByIndex(const std::vector<std::shared_ptr<Entity>>& group, int targetIndex) {
    int currentAliveIndex = 1; // Visual index starts at 1
    
    for (size_t i = 0; i < group.size(); i++) {
        if (group[i]->IsAlive()) {
            if (currentAliveIndex == targetIndex) {
                return group[i]; // Target found!
            }
            currentAliveIndex++;
        }
    }
    return nullptr; // Target not found or out of bounds
}