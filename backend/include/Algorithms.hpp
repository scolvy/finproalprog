#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>
#include <memory>
#include <string>
#include "Entities.hpp"

class Algorithms {
private:
    
    static int Partition(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high);

public:
    
    static void QuickSortInitiative(std::vector<std::shared_ptr<Entity>>& combatants, int low, int high);
    
    
    
    static std::shared_ptr<Entity> FindTargetByIndex(const std::vector<std::shared_ptr<Entity>>& group, int targetIndex);
};

#endif