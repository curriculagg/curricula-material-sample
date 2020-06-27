#ifndef BANKING_HPP
#define BANKING_HPP

#include <vector>
#include <string>
#include <iostream>
#include "types.hpp"
#include "command.hpp"
#include "models/account.hpp"

class Banking
{
public:
    // Construction, destruction
    ~Banking();

    // Attach a new command to the banking systemm
    template<class TCommand>
    void attach(std::string name)
    {
        Command* instance = new TCommand();
        this->commands.insert(std::pair<std::string, Command*>(name, instance));
    }

    // Invoked from program entry
    int main(std::vector<std::string> args);

private:
    command_map commands;
    account_map accounts;
};

#endif
