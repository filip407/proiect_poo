#include "YouthTeam.h"

YouthTeam::YouthTeam(const std::string& name) : TeamBase(name) {}

void YouthTeam::displayTeamType() const {
    std::cout << "Youth Team" << "\n" ;
}
