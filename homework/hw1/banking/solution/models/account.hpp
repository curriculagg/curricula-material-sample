#ifndef MODELS_ACCOUNT_HPP
#define MODELS_ACCOUNT_HPP

#include <string>
#include <vector>
#include "transaction.hpp"

class Account
{
public:
    Account(std::string path, std::string name, std::string transactions);
    ~Account();

    std::string name;
    std::vector<Transaction> transactions;

    static Account* read(std::string path);

private:
    std::string path;
};

#endif
