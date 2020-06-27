#include <iostream>
#include "../command.hpp"
#include "../types.hpp"

class HelpCommand : public Command
{
    virtual int main(std::vector<std::string>, account_map) const override
    {
        std::cout << "Help me!" << std::endl;
        return 0;
    }
};
