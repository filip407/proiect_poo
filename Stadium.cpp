#include "Stadium.h"
#include <iostream>

Stadium::Stadium(const std::string& name, int capacity)
    : name(name), capacity(capacity) {}
std::string Stadium::getName() const {
    return name;
}
int Stadium::getCapacity() const {
    return capacity;
}
void Stadium::displayStadiumInfo() const {
    std::cout << "Stadium: " << name << " (Capacitate: " << capacity << ")\n";
}
