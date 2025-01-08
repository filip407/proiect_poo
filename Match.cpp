#include "Match.h"

Match::Match(TeamBase& t1, TeamBase& t2, int g1, int g2)
    : team1(&t1), team2(&t2), goals1(g1), goals2(g2) {
    team1->updateResults(g1, g2);
    team2->updateResults(g2, g1);
}
// Implementari getter
TeamBase& Match::getTeam1() const {
    return *team1;
}
TeamBase& Match::getTeam2() const {
    return *team2;
}
int Match::getGoals1() const {
    return goals1;
}
int Match::getGoals2() const {
    return goals2;
}
// Suprascriere de operator <<
std::ostream& operator<<(std::ostream& os, const Match& match) {
    os << match.team1->getName() << " " << match.goals1 << " - "
       << match.goals2 << " " << match.team2->getName();
    return os;
}
