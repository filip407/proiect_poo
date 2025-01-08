#ifndef LEAGUEEXCEPTION_H
#define LEAGUEEXCEPTION_H

#include <exception>
#include <string>

class LeagueException : public std::exception {
protected:
    std::string message;

public:
    explicit LeagueException(const std::string& msg) : message(msg) {}
    //virtual ~LeagueException() noexcept {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
