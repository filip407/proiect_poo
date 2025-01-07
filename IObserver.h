#ifndef IOBSERVER_H
#define IOBSERVER_H

class IObserver {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~IObserver() = default;
};

#endif
