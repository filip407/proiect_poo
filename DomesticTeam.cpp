#include "DomesticTeam.h"

DomesticTeam::DomesticTeam(const std::string& name) : TeamBase(name) {}

void DomesticTeam::displayTeamType() const {
    std::cout << "Domestic Team" << "\n" ;
}
