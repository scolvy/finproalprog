#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>
#include <memory>
#include <string>
#include "Entities.hpp"

class Algorithms {
private:
    // Helper for Quick Sort
    static int Partition(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high);

public:
    // 1. MANUAL SORTING: Quick Sort (Sorts by DEX descending)
    static void QuickSortInitiative(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high);
    
    // 2. MANUAL SEARCHING: Linear Search (Find target by string name)
    // MANUAL SEARCHING: Linear Search by visual combat number (1-5)
    static std::shared_ptr<Entity> FindTargetByIndex(const std::vector<std::shared_ptr<Entity>>& group, int targetIndex);
};

#endif