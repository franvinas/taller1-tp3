#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
protected:
    std::string queue_name;

public:
    // Command();
    /*
     *  
     */
    virtual void operate() = 0;
    virtual ~Command();
};

class Define: public Command {
public:
    Define(const std::string &queue_name);
    virtual void operate() override;
};

class Push: public Command {
private:
    std::string message;
public:
    Push(const std::string &queue_name, const std::string &message);
    virtual void operate() override;
};

class Pop: public Command {
public:
    Pop(const std::string &queue_name);
    virtual void operate() override;
};

#endif