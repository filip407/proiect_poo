#include "InternationalTeam.h"

InternationalTeam::InternationalTeam(const std::string& name) : TeamBase(name) {}

void InternationalTeam::displayTeamType() const {
    std::cout << "International Team" << "\n" ;
}
