#pragma once

#include <string>
#include <variant>

enum struct reservedToken {
    colon,
    comma,

    openSquareBracket,
    closeSquareBracket,

    openCurlyBracket,
    closeCurlyBracket,
};

struct identifier {
    std::string name; 
};

struct eof {};

class Token {
    private:
        using tokenValueType = std::variant<double, std::string, reservedToken, identifier, eof>;
        tokenValueType _value;

    public:
        Token(tokenValueType value);

};