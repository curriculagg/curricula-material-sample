#include <string>
#include "decimal.hpp"

typedef decimal::Decimal<4> decimal_type;

decimal_type evaluate(const std::string& input);
