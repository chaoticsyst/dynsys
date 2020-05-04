#pragma once

#include <stdexcept>

namespace Parser {

class ParserException final : public std::logic_error {
    using std::logic_error::logic_error;
};

} //namespace Parser
