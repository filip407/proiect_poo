#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include "LeagueException.h"

class FileException : public LeagueException {
public:
    explicit FileException(const std::string& msg) : LeagueException(msg) {}
};

#endif
