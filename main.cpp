#include "League.h"
#include "FileException.h"
#include "TeamNotFoundException.h"
#include "MatchFormatException.h"
#include "DomesticTeam.h"
#include "YouthTeam.h"
#include "InternationalTeam.h"
#include "PremiumStadium.h"
#include <iostream>
#include <memory>
#include <limits>
#include "Statistics.h"

void displayStatistics(League& league) {
    std::string teamName;
    std::cout << "Introduceti numele echipei: ";
    std::getline(std::cin, teamName);
    auto team = league.findTeam(teamName);
    if (!team) {
        std::cout << "Echipa nu a fost gasita!\n";
        return;
    }
    // Afisam tipul echipei
    std::cout << "Tipul echipei: ";
    team->displayTeamType();
    // Foloseste suprasciere de operator pentru a afisa punctele si golurile
    std::cout << *team << std::endl;
    // Ia diferenta de goluri din statistici
    Statistic<int> goalDiffStat("Diferenta de goluri", team->getGoalDifference());
    // Afisam statisticile
    goalDiffStat.display();
}


void displayMenu() {
    //Afisam meniul din terminal
    std::cout << "\n=== Liga de Fotbal - Meniu ===\n";
    std::cout << "1. Afiseaza clasamentul\n";
    std::cout << "2. Afiseaza toate meciurile\n";
    std::cout << "3. Afiseaza statisticile echipelor\n";
    std::cout << "4. Adauga o echipa noua\n";
    std::cout << "5. Adauga un meci nou\n";
    std::cout << "6. Afiseaza statisticile unei echipe\n";
    std::cout << "7. Iesire\n";
    std::cout << "\nAlegeti o optiune (1-7): ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void addNewTeam(League& league) {
    //Adaugarea unei echipe
    std::string teamName, stadiumName;
    int teamType, capacity;
    char hasVIP;
    std::cout << "Introduceti numele echipei: ";
    std::getline(std::cin, teamName);
    std::cout << "Alegeti tipul echipei:\n";
    std::cout << "1. Echipa de tineret\n";
    std::cout << "2. Echipa domestica\n";
    std::cout << "3. Echipa internationala\n";
    std::cout << "Optiunea dvs: ";
    std::cin >> teamType;
    clearInputBuffer();
    std::shared_ptr<TeamBase> newTeam;
    switch (teamType) {
        case 1:
            newTeam = std::make_shared<YouthTeam>(teamName);
        break;
        case 2:
            newTeam = std::make_shared<DomesticTeam>(teamName);
        break;
        case 3:
            newTeam = std::make_shared<InternationalTeam>(teamName);
        break;
        default:
            std::cout << "Tip de echipa invalid!\n";
        return;
    }
    std::cout << "Introduceti numele stadionului echipei: ";
    std::getline(std::cin, stadiumName);
    std::cout << "Introduceti capacitatea stadionului: ";
    std::cin >> capacity;
    std::cout << "Are zona VIP? (y/n): ";
    std::cin >> hasVIP;
    clearInputBuffer();
    std::shared_ptr<Stadium> stadium;
    if (hasVIP == 'y' || hasVIP == 'Y') {
        stadium = std::make_shared<PremiumStadium>(stadiumName, capacity, true);
    } else {
        stadium = std::make_shared<Stadium>(stadiumName, capacity);
    }
    league.addTeam(newTeam);
    league.addStadium(teamName, stadium);
    std::cout << "Echipa si stadionul au fost adaugate cu succes!\n";
}

void handleUserChoice(League& league) {
    //Manipularea inputului uitlizatorului
    int choice;
    while (true) {
        displayMenu();

        if (!(std::cin >> choice)) {
            std::cout << "Va rugam introduceti un numar valid.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        std::cout << "\n";
        try {
            switch (choice) {
                case 1:
                    std::cout << "=== Clasament ===\n";
                league.displayStandings();
                break;
                case 2:
                    std::cout << "=== Meciuri ===\n";
                league.displayMatches();
                break;
                case 3:
                    std::cout << "=== Statistici Echipe ===\n";
                league.displayTeamStatistics();
                break;
                case 4:
                    std::cout << "=== Adauga Echipa Noua ===\n";
                addNewTeam(league);
                break;
                case 5:
                    std::cout << "=== Adauga Meci Nou ===\n";
                std::cin >> league;
                break;
                case 6:
                    std::cout << "=== Statistici pentru o echipa ===\n";
                displayStatistics(league);
                break;
                case 7:
                    std::cout << "La revedere!\n";
                return;
                default:
                    std::cout << "Optiune invalida. Va rugam alegeti un numar intre 1 si 7.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
        std::cout << "\nApasati Enter pentru a continua...";
        std::cin.get();
        std::cout << "\033[2J\033[1;1H";
    }
}

int main() {
    try {
        League& league = League::getInstance();
        league.loadFromFile("tastatura.txt");
        std::cout << "Bine ati venit la aplicatia Liga de Fotbal!\n";
        handleUserChoice(league);
    //Erori
    } catch (const FileException& e) {
        std::cerr << "Eroare fisier: " << e.what() << "\n";
    } catch (const TeamNotFoundException& e) {
        std::cerr << "Echipa negasita: " << e.what() << "\n";
    } catch (const MatchFormatException& e) {
        std::cerr << "Eroare format meci: " << e.what() << "\n";
    } catch (const LeagueException& e) {
        std::cerr << "Eroare liga: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Eroare neasteptata: " << e.what() << "\n";
    }
    return 0;
}
