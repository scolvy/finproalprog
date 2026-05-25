#ifndef EVENTSYSTEM_HPP
#define EVENTSYSTEM_HPP

#include <memory>
#include "Entities.hpp"

// ENKAPSULASI: A static utility class specifically for handling Narrative Nodes
class EventSystem {
public:
    // Takes the player character and current Scrap (passed by reference to modify it)
    static void ResolveRandomEvent(std::shared_ptr<Entity> player, int& partyScrap);

private:
    // Specific Event Scenarios from your GDD
    static void EventCaveIn(std::shared_ptr<Entity> player, int& partyScrap);
    static void EventMutatedAlchemist(std::shared_ptr<Entity> player);
    static void EventBlackMarket(std::shared_ptr<Entity> player);
};

#endif