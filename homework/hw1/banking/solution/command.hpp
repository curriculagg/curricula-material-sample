#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <string>
#include "types.hpp"

class Command
{
public:
    virtual ~Command() {}
    virtual int main(std::vector<std::string>, account_map) const = 0;
};

#endif
