#include <utility>

#include<tokens.hpp>

using namespace Json;

Token::Token(tokenValueType value) :
    _value(std::move(value))
    {

    }

bool Token::isEOF() const {
    return std::holds_alternative<eof>(_value);
}

bool Token::isNumber() const {
    return std::holds_alternative<double>(_value);
}

bool Token::isSeparator() const {
    return std::holds_alternative<reservedToken>(_value);
}

bool Token::isString() const {
    return std::holds_alternative<std::string>(_value);
}

double Token::getNumber() const {
    return std::get<double>(_value);
}

std::string Token::getString() const {
    return std::get<std::string>(_value);
}

reservedToken Token::getSeparator() const {
    return std::get<reservedToken>(_value);
}

