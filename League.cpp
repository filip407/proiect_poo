#include "League.h"
#include "FileException.h"
#include "TeamNotFoundException.h"
#include "MatchFormatException.h"
#include "DomesticTeam.h"
#include "YouthTeam.h"
#include "InternationalTeam.h"
#include "PremiumStadium.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <map>
#include <iomanip>
#include <limits>

League* League::instance = nullptr;
//implementarea Singleton Pattern
League& League::getInstance() {
    if (instance == nullptr) {
        instance = new League();
    }
    return *instance;
}

void League::addTeam(const std::shared_ptr<TeamBase>& team) {
    teams.push_back(team);
    //Notificam observatorii
    notifyObservers("Echipa noua adaugata: " + team->getName());
}

//Suprascrierea de operator de citire
std::istream& operator>>(std::istream& is, League& league) {
    std::string team1_name, team2_name;
    int goals1, goals2;
    std::cout << "Introduceti numele primei echipe: ";
    std::getline(is, team1_name);
    std::cout << "Introduceti numele celei de-a doua echipe: ";
    std::getline(is, team2_name);
    std::cout << "Introduceti golurile primei echipe: ";
    is >> goals1;
    std::cout << "Introduceti golurile celei de-a doua echipe: ";
    is >> goals2;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    try {
        league.addMatch(team1_name, team2_name, goals1, goals2);
    } catch (const TeamNotFoundException& e) {
        std::cerr << "Eroare: " << e.what() << "\n";
    }
    return is;
}

std::map<std::string, League::TeamStatistics> League::calculateTeamStatistics() const {
    std::map<std::string, TeamStatistics> stats;

    //Initializam statisticile pentru toate echipele
    for (const auto& team : teams) {
        stats[team->getName()] = TeamStatistics();
    }

    //Calculam statisticile pentru fiecare meci
    for (const auto& match : matches) {
        const std::string& team1 = match.getTeam1().getName();
        const std::string& team2 = match.getTeam2().getName();
        int goals1 = match.getGoals1();
        int goals2 = match.getGoals2();

        // Actualizam statisticile pentru prima echipa
        stats[team1].matchesPlayed++;
        stats[team1].goalsScored += goals1;
        stats[team1].goalsConceded += goals2;

        //Actualizam statisticile pentru a doua echipa
        stats[team2].matchesPlayed++;
        stats[team2].goalsScored += goals2;
        stats[team2].goalsConceded += goals1;

        //Actualizam rezultatele
        if (goals1 > goals2) {
            stats[team1].wins++;
            stats[team2].losses++;
        } else if (goals1 < goals2) {
            stats[team1].losses++;
            stats[team2].wins++;
        } else {
            stats[team1].draws++;
            stats[team2].draws++;
        }
    }
    // Calculam mediile si ratele pentru fiecare echipa
    for (auto& [teamName, teamStats] : stats) {
        if (teamStats.matchesPlayed > 0) {
            teamStats.averageGoalsScored = static_cast<double>(teamStats.goalsScored) / teamStats.matchesPlayed;
            teamStats.averageGoalsConceded = static_cast<double>(teamStats.goalsConceded) / teamStats.matchesPlayed;
            teamStats.winRate = static_cast<double>(teamStats.wins) / teamStats.matchesPlayed * 100.0;
        }
    }
    return stats;
}

void League::displayTeamStatistics() const {
    auto stats = calculateTeamStatistics();
    std::cout << std::string(120, '-') << "\n";
    std::cout << std::left
              << std::setw(20) << "Echipa"
              << std::setw(10) << "Jucate"
              << std::setw(10) << "Date"
              << std::setw(10) << "Incasate"
              << std::setw(20) << "Avg Goluri Date"
              << std::setw(20) << "Avg Goluri Luate"
              << std::setw(20) << "Rata de castig %"
              << "\n";
    std::cout << std::string(120, '-') << "\n";
    for (const auto& [teamName, teamStats] : stats) {
        std::cout << std::left
                  << std::setw(20) << teamName
                  << std::setw(10) << teamStats.matchesPlayed
                  << std::setw(10) << teamStats.goalsScored
                  << std::setw(10) << teamStats.goalsConceded
                  << std::fixed << std::setprecision(2)
                  << std::setw(20) << teamStats.averageGoalsScored
                  << std::setw(20) << teamStats.averageGoalsConceded
                  << std::setw(20) << teamStats.winRate
                  << "\n";
    }
    std::cout << std::string(120, '-') << "\n";
}

//Citirea din fisier
void League::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileException("Error: Fisierul nu a putut fi deschis." + filename);
    }
    std::string line;
    bool readingTeams = true;
    bool readingStadiums = false;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line == "Stadioane:") {
            readingTeams = false;
            readingStadiums = true;
            continue;
        }
        if (line == "Meciuri:") {
            readingStadiums = false;
            continue;
        }
        if (readingTeams) {
            if (line.find("Tineret") != std::string::npos) {
                teams.push_back(std::make_shared<YouthTeam>(line));
            } else if (line.find("Int") != std::string::npos) {
                teams.push_back(std::make_shared<InternationalTeam>(line));
            } else {
                teams.push_back(std::make_shared<DomesticTeam>(line));
            }
        } else if (readingStadiums) {
            size_t delimPos = line.find(':');
            if (delimPos != std::string::npos) {
                std::string teamName = line.substr(0, delimPos);
                std::string stadiumData = line.substr(delimPos + 1);
                size_t comma1 = stadiumData.find(',');
                size_t comma2 = stadiumData.find_last_of(',');
                if (comma1 != std::string::npos && comma2 != std::string::npos && comma1 != comma2) {
                    std::string stadiumName = stadiumData.substr(0, comma1);
                    int capacity = std::stoi(stadiumData.substr(comma1 + 1, comma2 - comma1 - 1));
                    char hasVIP = stadiumData[comma2 + 1];
                    std::shared_ptr<Stadium> stadium;
                    if (hasVIP == 'y' || hasVIP == 'Y') {
                        stadium = std::make_shared<PremiumStadium>(stadiumName, capacity, true);
                    } else {
                        stadium = std::make_shared<Stadium>(stadiumName, capacity);
                    }
                    addStadium(teamName, stadium);
                } else {
                    throw MatchFormatException("Sintaxa gresita stadion: " + line);
                }
            }
        } else {
            std::regex matchPattern(R"((.+)\s+(\d+)\s+(.+)\s+(\d+))");
            std::smatch matchResult;
            if (std::regex_match(line, matchResult, matchPattern)) {
                std::string team1_name = matchResult[1].str();
                int goals1 = std::stoi(matchResult[2].str());
                std::string team2_name = matchResult[3].str();
                int goals2 = std::stoi(matchResult[4].str());
                addMatch(team1_name, team2_name, goals1, goals2);
            } else {
                throw MatchFormatException("Sintaxa gresita meciuri: " + line);
            }
        }
    }
    file.close();
}

// Afisarea meciurilor
void League::displayMatches() const {
    std::cout << "Matches:\n";
    for (const auto& match : matches) {
        std::cout << match;
        auto stadium = getStadium(match.getTeam1().getName());
        if (stadium) {
            std::cout << " at ";
            stadium->displayStadiumInfo();
        } else {
            std::cout << " (Stadion necunoscut)\n";
        }
    }
}

void League::addStadium(const std::string& teamName, std::shared_ptr<Stadium> stadium) {
    stadiums[teamName] = stadium;
}

std::shared_ptr<Stadium> League::getStadium(const std::string& teamName) const {
    auto it = stadiums.find(teamName);
    if (it != stadiums.end()) {
        return it->second;
    }
    return nullptr;
}

void League::displayStandings() const {
    if (matches.empty()) {
        throw LeagueException("Nu au fost meciuri gasite, clasamentul nu poate fi afisat.");
    }
    std::cout << std::string(100, '-') << "\n";
    std::cout << "Nume Echipa                 Puncte   Jucate  Castigate  Egaluri  Pierdute  Diferenta de goluri\n";
    std::cout << std::string(100, '-') << "\n";
    std::map<std::string, int> points;
    std::map<std::string, int> wins;
    std::map<std::string, int> draws;
    std::map<std::string, int> losses;
    std::map<std::string, int> played;
    std::map<std::string, int> goalDifference;

    //Initializare statistici
    for (const auto& team : teams) {
        points[team->getName()] = 0;
        wins[team->getName()] = 0;
        draws[team->getName()] = 0;
        losses[team->getName()] = 0;
        played[team->getName()] = 0;
        goalDifference[team->getName()] = 0;
    }

    //Calculare de statistici
    for (const auto& match : matches) {
        const auto& team1 = match.getTeam1();
        const auto& team2 = match.getTeam2();
        int goals1 = match.getGoals1();
        int goals2 = match.getGoals2();
        played[team1.getName()]++;
        played[team2.getName()]++;
        goalDifference[team1.getName()] += (goals1 - goals2);
        goalDifference[team2.getName()] += (goals2 - goals1);
        if (goals1 > goals2) {
            points[team1.getName()] += 3;
            wins[team1.getName()]++;
            losses[team2.getName()]++;
        } else if (goals1 < goals2) {
            points[team2.getName()] += 3;
            wins[team2.getName()]++;
            losses[team1.getName()]++;
        } else {
            points[team1.getName()] += 1;
            points[team2.getName()] += 1;
            draws[team1.getName()]++;
            draws[team2.getName()]++;
        }
    }
    //Sorteaza echipele
    std::vector<std::pair<std::string, std::tuple<int, int>>> teamStats;
    for (const auto& entry : points) {
        teamStats.push_back({entry.first, std::make_tuple(entry.second, goalDifference[entry.first])});
    }
    std::sort(teamStats.begin(), teamStats.end(),
        [](const auto& lhs, const auto& rhs) {
            const auto& [lhsPoints, lhsGoalDiff] = lhs.second;
            const auto& [rhsPoints, rhsGoalDiff] = rhs.second;
            return lhsPoints > rhsPoints || (lhsPoints == rhsPoints && lhsGoalDiff > rhsGoalDiff);
        });
    //Afiseaza rezultatele
    for (const auto& teamStat : teamStats) {
        const std::string& teamName = teamStat.first;
        const auto& [teamPoints, teamGoalDiff] = teamStat.second;
        std::cout << std::left << std::setw(30) << teamName
                  << std::setw(10) << teamPoints
                  << std::setw(10) << played[teamName]
                  << std::setw(10) << wins[teamName]
                  << std::setw(10) << draws[teamName]
                  << std::setw(10) << losses[teamName]
                  << std::setw(10) << teamGoalDiff << "\n";
    }
    std::cout << std::string(100, '-') << "\n";
}

void League::addMatch(const std::string& team1_name, const std::string& team2_name, int goals1, int goals2) {
    auto t1 = findTeam(team1_name);
    auto t2 = findTeam(team2_name);
    if (t1 && t2) {
        matches.emplace_back(*t1, *t2, goals1, goals2);
        //Notificam observatorii
        notifyObservers("Meci adaugat: " + team1_name + " vs " + team2_name);
    } else {
        throw TeamNotFoundException("Una sau ambele echipe nu au fost gasite: " + team1_name + " or " + team2_name);
    }
}

std::shared_ptr<TeamBase> League::findTeam(const std::string& name) {
    auto it = std::find_if(teams.begin(), teams.end(),
        [&name](const std::shared_ptr<TeamBase>& team) {
            return team->getName() == name;
        });

    if (it != teams.end()) {
        return *it;
    }
    return nullptr;
}

void League::removeTeam(const std::string& teamName) {
    // Gasim si stergem echipa
    auto it = std::find_if(teams.begin(), teams.end(),
        [&teamName](const std::shared_ptr<TeamBase>& team) {
            return team->getName() == teamName;
        });
    if (it == teams.end()) {
        throw TeamNotFoundException("Echipa " + teamName + " nu a fost gasita!");
    }
    // Stergem toate meciurile echipei
    matches.erase(
        std::remove_if(matches.begin(), matches.end(),
            [&teamName](const Match& match) {
                return match.getTeam1().getName() == teamName ||
                       match.getTeam2().getName() == teamName;
            }),
        matches.end());
    // Stergem stadionul echipei
    stadiums.erase(teamName);
    // Stergem echipa
    teams.erase(it);
    //Notificam observatorii
    notifyObservers("Echipa " + teamName + " a fost eliminata din liga");
    std::cout << "Echipa " << teamName << " a fost eliminata cu succes!\n";
}

void League::resetSeason() {
    matches.clear();  // Stergem toate meciurile
    // Resetam statisticile pentru toate echipele
    for (auto& team : teams) {
        team = std::make_shared<DomesticTeam>(team->getName());  // Recream echipa pastrand doar numele
        team->updateResults(0, 0);  // Resetam rezultatele
    }
    // Notificam observatorii
    notifyObservers("Sezonul a fost resetat");
    std::cout << "Sezonul a fost resetat cu succes!\n";
}

void League::removeMatch(size_t index) {
    if (index >= matches.size()) {
        throw LeagueException("Index invalid pentru stergerea meciului.");
    }
    // Anulam rezultatele pentru ambele echipe
    Match& match = matches[index];
    match.getTeam1().reverseResults(match.getGoals1(), match.getGoals2());
    match.getTeam2().reverseResults(match.getGoals2(), match.getGoals1());
    // Notificam observatorii
    notifyObservers("Meci sters: " + match.getTeam1().getName() + " vs " + match.getTeam2().getName());
    // Stergem meciul din vector
    matches.erase(matches.begin() + index);
}

void League::displayMatchesWithIndex() const {
    std::cout << "Lista meciurilor:\n";
    for (size_t i = 0; i < matches.size(); ++i) {
        std::cout << "[" << i << "] " << matches[i];
        auto stadium = getStadium(matches[i].getTeam1().getName());
        if (stadium) {
            std::cout << " at ";
            stadium->displayStadiumInfo();
        } else {
            std::cout << " (Stadion necunoscut)\n";
        }
    }
}