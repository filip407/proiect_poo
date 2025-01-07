#ifndef TEAMBASE_H
#define TEAMBASE_H

#include <string>
#include <iostream>
#include "IObserver.h"

class TeamBase : public IObserver {
protected:
    std::string name;
    int points;
    int goals_scored;
    int goals_conceded;

public:
    TeamBase(const TeamBase& other);
    // Operator de atribuire
    TeamBase& operator=(const TeamBase& other);
    void update(const std::string& message) override;
    TeamBase(const std::string& name = "");
    virtual ~TeamBase() {}
    virtual void updateResults(int scored, int conceded);
    virtual void displayTeamType() const = 0;
    std::string getName() const;
    int getPoints() const;
    int getGoalDifference() const;
    friend std::ostream& operator<<(std::ostream& os, const TeamBase& team);
};

#endif
