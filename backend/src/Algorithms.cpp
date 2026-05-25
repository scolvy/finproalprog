#include "Algorithms.hpp"

int Algorithms::Partition(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high) {
    
    int pivotValue = combatants[high]->GetTotalStat(DEX); 
    int i = low - 1;

    for (int j = low; j < high; j++) {
        
        if (combatants[j]->GetTotalStat(DEX) >= pivotValue) {
            i++;
            
            std::shared_ptr<Entity> temp = combatants[i];
            combatants[i] = combatants[j];
            combatants[j] = temp;
        }
    }
    
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

std::shared_ptr<Entity> Algorithms::FindTargetByIndex(const std::vector<std::shared_ptr<Entity>>& group, int targetIndex) {
    int currentAliveIndex = 1; 
    
    for (size_t i = 0; i < group.size(); i++) {
        if (group[i]->IsAlive()) {
            if (currentAliveIndex == targetIndex) {
                return group[i]; 
            }
            currentAliveIndex++;
        }
    }
    return nullptr; 
}