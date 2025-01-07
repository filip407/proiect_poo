#ifndef STATISTIC_H
#define STATISTIC_H

#include <iostream>
#include <string>

//Clasa template
template<typename T>
class Statistic {
    std::string name;
    T value;

public:
    // Constructor
    Statistic(const std::string& name, T value) : name(name), value(value) {}
    // Getteri
    std::string getName() const { return name; }
    T getValue() const { return value; }
    // Setteri
    void setValue(T newValue) { value = newValue; }
    // Afisare
    void display() const {
        std::cout << "Statistic - " << name << ": " << value << "\n";
    }
};
#endif
