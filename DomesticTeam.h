#ifndef DOMESTICTEAM_H
#define DOMESTICTEAM_H
#include "TeamBase.h"

class DomesticTeam : public TeamBase {
public:
    DomesticTeam(const std::string& name);
    void displayTeamType() const override;
};

#endif
