#ifndef LEAGUE_H
#define LEAGUE_H

#include "TeamBase.h"
#include "Match.h"
#include "Stadium.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

class League {
    struct TeamStatistics {
        int matchesPlayed = 0;
        int goalsScored = 0;
        int goalsConceded = 0;
        int wins = 0;
        int draws = 0;
        int losses = 0;
        double averageGoalsScored = 0.0;
        double averageGoalsConceded = 0.0;
        double winRate = 0.0;
    };
    std::map<std::string, TeamStatistics> calculateTeamStatistics() const;
    static League* instance;
    std::vector<std::shared_ptr<TeamBase>> teams;
    std::vector<Match> matches;
    std::map<std::string, std::shared_ptr<Stadium>> stadiums;
    void notifyObservers(const std::string& message) {
        for (auto& team : teams) {
            team->update(message);
        }
    }
    League() = default;
public:
    void displayTeamStatistics() const;
    League(const League&) = delete;
    League& operator=(const League&) = delete;
    static League& getInstance();
    void loadFromFile(const std::string& filename);
    void addMatch(const std::string& team1_name, const std::string& team2_name, int goals1, int goals2);
    void displayStandings() const;
    void displayMatches() const;
    void addStadium(const std::string& teamName, std::shared_ptr<Stadium> stadium);
    std::shared_ptr<Stadium> getStadium(const std::string& teamName) const;
    void addTeam(std::shared_ptr<TeamBase> team);
    friend std::istream& operator>>(std::istream& is, League& league);
    std::shared_ptr<TeamBase> findTeam(const std::string& name);
};

#endif