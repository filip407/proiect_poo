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
    std::vector<std::shared_ptr<TeamBase>> teams;
    std::vector<Match> matches;
    std::map<std::string, std::shared_ptr<Stadium>> stadiums;
    void notifyObservers(const std::string& message) {
        for (auto& team : teams) {
            team->update(message);
        }
    }
    //instanta unica
    static League* instance;
    //constructor privat
    League() = default;
public:
    //prevenirea copierii
    League(const League&) = delete;
    //prevenirea atribuirii
    League& operator=(const League&) = delete;
    //metoda statica de acces
    static League& getInstance();
    void loadFromFile(const std::string& filename);
    void addMatch(const std::string& team1_name, const std::string& team2_name, int goals1, int goals2);
    void displayStandings() const;
    void displayMatches() const;
    void addStadium(const std::string& teamName, std::shared_ptr<Stadium> stadium);
    std::shared_ptr<Stadium> getStadium(const std::string& teamName) const;
    void addTeam(const std::shared_ptr<TeamBase>& team);
    friend std::istream& operator>>(std::istream& is, League& league);
    std::shared_ptr<TeamBase> findTeam(const std::string& name);
    void displayTeamStatistics() const;
    void resetSeason();
    void removeTeam(const std::string& teamName);
    void removeMatch(size_t index);
    void displayMatchesWithIndex() const;
};

#endif