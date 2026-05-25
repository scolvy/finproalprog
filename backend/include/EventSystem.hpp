#ifndef EVENTSYSTEM_HPP
#define EVENTSYSTEM_HPP

#include <memory>
#include "Entities.hpp"


class EventSystem {
public:
    
    static void ResolveRandomEvent(std::shared_ptr<Entity> player, int& partyScrap);

private:
    
    static void EventCaveIn(std::shared_ptr<Entity> player, int& partyScrap);
    static void EventMutatedAlchemist(std::shared_ptr<Entity> player);
    static void EventBlackMarket(std::shared_ptr<Entity> player);
};

#endif