#ifndef INTERNATIONALTEAM_H
#define INTERNATIONALTEAM_H

#include "TeamBase.h"

class InternationalTeam : public TeamBase {
public:
    InternationalTeam(const std::string& name);
    void displayTeamType() const override;
};

#endif
