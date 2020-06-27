#include <vector>
#include <string>
#include "banking.hpp"
#include "commands/help.hpp"

using namespace std;

int main(int argc, char** argv)
{
    vector<string> args(argv + 1, argv + argc);
    Banking banking;
    banking.attach<HelpCommand>("help");
    return banking.main(args);
}
