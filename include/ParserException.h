#pragma once

namespace Parser {

class ParserException final : std::logic_error {
    using std::logic_error::logic_error;
};

} //namespace Parser
