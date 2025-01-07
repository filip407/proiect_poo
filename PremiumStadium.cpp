#include "PremiumStadium.h"
#include <iostream>

PremiumStadium::PremiumStadium(const std::string& name, int capacity, bool hasVIP)
    : Stadium(name, capacity), hasVIPArea(hasVIP) {}

void PremiumStadium::displayStadiumInfo() const {
    std::cout << "Premium Stadium: " << name << " (Capacitate: " << capacity
              << ", Zona VIP: " << (hasVIPArea ? "DA" : "NU") << ")\n";
}