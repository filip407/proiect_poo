#ifndef PREMIUMSTADIUM_H
#define PREMIUMSTADIUM_H

#include "Stadium.h"

class PremiumStadium : public Stadium {
    bool hasVIPArea;
    
public:
    PremiumStadium(const std::string& name, int capacity, bool hasVIP);
    void displayStadiumInfo() const override;
};

#endif