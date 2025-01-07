#ifndef MATCHFORMATEXCEPTION_H
#define MATCHFORMATEXCEPTION_H

#include "LeagueException.h"

class MatchFormatException : public LeagueException {
public:
    explicit MatchFormatException(const std::string& msg) : LeagueException(msg) {}
};

#endif
