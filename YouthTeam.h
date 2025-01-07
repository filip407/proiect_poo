#ifndef YOUTHTEAM_H
#define YOUTHTEAM_H

#include "TeamBase.h"

class YouthTeam : public TeamBase {
public:
    YouthTeam(const std::string& name);
    void displayTeamType() const override;
};

#endif
