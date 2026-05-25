#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>




enum NodeType { 
    COMBAT,   
    ELITE,    
    EVENT,    
    MERCHANT, 
    CAMP      
};




class MapNode {
public:
    int depthLevel;
    NodeType type;
    std::string roomName;
    std::string symbol; 
    
    
    std::shared_ptr<MapNode> leftPath;
    std::shared_ptr<MapNode> rightPath;

    MapNode(int depth, NodeType t, std::string name, std::string sym);
};




class MapTree {
private:
    std::shared_ptr<MapNode> root;
    std::shared_ptr<MapNode> currentNode;
    int maxDepth;

    
    std::shared_ptr<MapNode> GenerateBranch(int currentDepth);

public:
    MapTree(int depth);
    
    
    std::shared_ptr<MapNode> GetCurrentNode() const;
    void MoveLeft();
    void MoveRight();
    
    
    void PrintAvailablePaths() const;
    bool IsAtBoss() const;
};

#endif