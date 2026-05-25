#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

#include <string>
#include <map>
#include "Globals.hpp"

enum EquipSlot { WEAPON, ARMOR, RELIC };

struct Equipment {
    std::string name;
    EquipSlot slot;
    std::string description;
    
    // Maps a stat to a modifier (e.g., {INT: +5, STR: -2})
    std::map<StatType, int> statModifiers;

    // Default constructor for empty slots
    Equipment() : name("Empty"), slot(WEAPON), description("Nothing equipped.") {}
    
    Equipment(std::string n, EquipSlot s, std::string desc, std::map<StatType, int> mods)
        : name(n), slot(s), description(desc), statModifiers(mods) {}
};

#endif