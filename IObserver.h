#ifndef IOBSERVER_H
#define IOBSERVER_H

//clasa pentru Observer Pattern
class IObserver {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~IObserver() = default;
};

#endif
