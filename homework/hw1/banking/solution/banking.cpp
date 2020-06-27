#include "banking.hpp"

Banking::~Banking()
{
    for (command_map::iterator it = this->commands.begin(); it != this->commands.end(); it++)
    {
        delete it->second;
    }
}

int Banking::main(std::vector<std::string> args)
{
    if (args.size() == 0)
    {
        std::cerr << "a command must be specified" << std::endl;
        return 1;
    }
    command_map::iterator it = this->commands.find(args[0]);
    if (it == this->commands.end())
    {
        std::cerr << "no matching command was found for " << args[0] << std::endl;
        return 1;
    }
    return it->second->main(args, this->accounts);
}
