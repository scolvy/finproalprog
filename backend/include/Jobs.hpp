#ifndef JOBS_HPP
#define JOBS_HPP

#include <string>
#include <vector>    
#include <memory>
#include "Globals.hpp"


class Entity; 

class Scrapper : public JobClass {
public:
    Scrapper();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Drifter : public JobClass {
public:
    Drifter();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Sparkmage : public JobClass {
public:
    Sparkmage();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};


class Apothecary : public JobClass {
public:
    Apothecary();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};


class Envoy : public JobClass {
public:
    Envoy();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Juggernaut : public JobClass {
public:
    Juggernaut();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};


class Phantom : public JobClass {
public:
    Phantom();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Voidcaller : public JobClass {
public:
    Voidcaller();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};



class FleshMender : public JobClass {
public:
    FleshMender();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Commander : public JobClass {
public:
    Commander();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};


class BloodBrawler : public JobClass {
public:
    BloodBrawler();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Gunslinger : public JobClass {
public:
    Gunslinger();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class Stormweaver : public JobClass {
public:
    Stormweaver();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};

class PlagueDoctor : public JobClass {
public:
    PlagueDoctor();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};


class Charlatan : public JobClass {
public:
    Charlatan();
    void ExecuteSkill(int skillIndex, std::shared_ptr<Entity> user, std::vector<std::shared_ptr<Entity>>& party, std::vector<std::shared_ptr<Entity>>& enemies) override;
};
#endif