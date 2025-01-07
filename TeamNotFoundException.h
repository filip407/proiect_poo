#ifndef TEAMNOTFOUNDEXCEPTION_H
#define TEAMNOTFOUNDEXCEPTION_H

#include "LeagueException.h"

class TeamNotFoundException : public LeagueException {
public:
    explicit TeamNotFoundException(const std::string& msg) : LeagueException(msg) {}
};

#endif
