#include "TeamBase.h"

TeamBase::TeamBase(const std::string& name)
    : name(name), points(0), goals_scored(0), goals_conceded(0) {}
//Suprascriere de operator =
TeamBase& TeamBase::operator=(const TeamBase& other) {
    if (this != &other) { // Protecție împotriva auto-atribuire
        name = other.name;
        points = other.points;
        goals_scored = other.goals_scored;
        goals_conceded = other.goals_conceded;
    }
    return *this;
}
void TeamBase::update(const std::string& message) {
    std::cout << name << " a primit update: " << message << "\n";
}
void TeamBase::updateResults(int scored, int conceded) {
    goals_scored += scored;
    goals_conceded += conceded;
    if (scored > conceded)
        points += 3;
    else if (scored == conceded)
        points += 1;
}
std::string TeamBase::getName() const { return name; }
int TeamBase::getPoints() const { return points; }
int TeamBase::getGoalDifference() const { return goals_scored - goals_conceded; }
//Suprascriere de operator <<
std::ostream& operator<<(std::ostream& os, const TeamBase& team) {
    os << "Points: " << team.points
       << ", Goals Scored: " << team.goals_scored
       << ", Goals Conceded: " << team.goals_conceded;
    return os;
}
