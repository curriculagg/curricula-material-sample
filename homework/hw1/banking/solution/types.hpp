#ifndef TYPES_HPP
#define TYPES_HPP

#include <unordered_map>
#include <string>

class Command;
class Account;

typedef std::unordered_map<std::string, Command*> command_map;
typedef std::unordered_map<std::string, Account*> account_map;

#endif
