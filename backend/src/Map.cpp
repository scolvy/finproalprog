#include "Map.hpp"

MapNode::MapNode(int depth, NodeType t, std::string name, std::string sym) 
    : depthLevel(depth), type(t), roomName(name), symbol(sym), leftPath(nullptr), rightPath(nullptr) {}



MapTree::MapTree(int depth) : maxDepth(depth) {
    root = GenerateBranch(1);
    currentNode = root;
}

std::shared_ptr<MapNode> MapTree::GenerateBranch(int currentDepth) {
    if (currentDepth > maxDepth) return nullptr;

    NodeType randomType;
    std::string name;
    std::string symbol;
    
    if (currentDepth == maxDepth) {
        randomType = ELITE; 
        name = "Boss Lair";
        symbol = "[ BOSS ]";
    } 
    else if (currentDepth == 1) {
        randomType = CAMP;
        name = "Desolate Entrance";
        symbol = "[ START ]";
    } 
    else {
        int roll = rand() % 100;
        
        if (roll < 40) {
            randomType = COMBAT; name = "Scrap Skirmish"; symbol = "[ C ]";
        } else if (roll < 55) {
            randomType = EVENT; name = "Unknown Signal"; symbol = "[ ? ]";
        } else if (roll < 70) {
            randomType = MERCHANT; name = "Scrap-Broker"; symbol = "[ $ ]";
        } else if (roll < 85) {
            randomType = CAMP; name = "Smoldering Campfire"; symbol = "[ R ]";
        } else {
            randomType = ELITE; name = "Elite Patrol"; symbol = "[ E ]";
        }
    }

    auto node = std::make_shared<MapNode>(currentDepth, randomType, name, symbol);

    node->leftPath = GenerateBranch(currentDepth + 1);
    node->rightPath = GenerateBranch(currentDepth + 1);

    return node;
}

std::shared_ptr<MapNode> MapTree::GetCurrentNode() const { 
    return currentNode; 
}

bool MapTree::IsAtBoss() const {
    return currentNode->depthLevel == maxDepth;
}

void MapTree::MoveLeft() {
    if (currentNode->leftPath) {
        currentNode = currentNode->leftPath;
    }
}

void MapTree::MoveRight() {
    if (currentNode->rightPath) {
        currentNode = currentNode->rightPath;
    }
}

void MapTree::PrintAvailablePaths() const {
    std::cout << "You are at: " << currentNode->symbol << " " << currentNode->roomName 
              << " (Layer " << currentNode->depthLevel << "/" << maxDepth << ")\n\n";
    
    if (IsAtBoss()) {
        std::cout << "There is nowhere left to run. The Boss awaits.\n";
        return;
    }

    std::cout << "The path splits ahead of you:\n";
    
    if (currentNode->leftPath) {
        std::cout << "[L] Go Left  -> " << currentNode->leftPath->symbol << " " << currentNode->leftPath->roomName << "\n";
    }
    if (currentNode->rightPath) {
        std::cout << "[R] Go Right -> " << currentNode->rightPath->symbol << " " << currentNode->rightPath->roomName << "\n";
    }
}