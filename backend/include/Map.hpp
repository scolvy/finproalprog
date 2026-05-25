#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>

// =========================================================
// NODE TYPES (From GDD Section 5)
// =========================================================
enum NodeType { 
    COMBAT,   // [ C ] Standard enemies, basic scrap
    ELITE,    // [ E ] Dangerous foes, high scrap/relics
    EVENT,    // [ ? ] Narrative stat checks
    MERCHANT, // [ $ ] Spend scrap on consumables
    CAMP      // [ R ] Heal or purge debuffs
};

// =========================================================
// MAP NODE (A single room in the dungeon)
// =========================================================
class MapNode {
public:
    int depthLevel;
    NodeType type;
    std::string roomName;
    std::string symbol; // The ASCII symbol for the map UI
    
    // Binary Tree paths
    std::shared_ptr<MapNode> leftPath;
    std::shared_ptr<MapNode> rightPath;

    MapNode(int depth, NodeType t, std::string name, std::string sym);
};

// =========================================================
// MAP TREE (The Procedural Generator)
// =========================================================
class MapTree {
private:
    std::shared_ptr<MapNode> root;
    std::shared_ptr<MapNode> currentNode;
    int maxDepth;

    // Recursive function to build the 15-layer branching paths
    std::shared_ptr<MapNode> GenerateBranch(int currentDepth);

public:
    MapTree(int depth);
    
    // Navigation
    std::shared_ptr<MapNode> GetCurrentNode() const;
    void MoveLeft();
    void MoveRight();
    
    // UI Helpers
    void PrintAvailablePaths() const;
    bool IsAtBoss() const;
};

#endif