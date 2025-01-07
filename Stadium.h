#ifndef STADIUM_H
#define STADIUM_H

#include <string>

class Stadium {
protected:
    std::string name;
    int capacity;
public:
    Stadium(const std::string& name, int capacity);
    virtual ~Stadium() = default;
    std::string getName() const;
    int getCapacity() const;
    virtual void displayStadiumInfo() const;
};

#endif 