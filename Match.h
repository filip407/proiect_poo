#ifndef MATCH_H
#define MATCH_H

#include "Teambase.h"
#include <memory>

class Match {
    TeamBase* team1;
    TeamBase* team2;
    int goals1;
    int goals2;

public:
    Match(TeamBase& t1, TeamBase& t2, int g1, int g2);
    // Functii getter
    TeamBase& getTeam1() const;
    TeamBase& getTeam2() const;
    int getGoals1() const;
    int getGoals2() const;
    friend std::ostream& operator<<(std::ostream& os, const Match& match);
};

#endif
