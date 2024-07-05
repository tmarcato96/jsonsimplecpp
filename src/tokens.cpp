#include <map>
#include <string_view>

#include<tokens.hpp>

const std::map<std::string_view, reservedToken> specialCharacterMap {
    {":", reservedToken::colon},
    {",", reservedToken::comma},
    
    {"[", reservedToken::openSquareBracket},
    {"]", reservedToken::closeSquareBracket},

    {"{", reservedToken::openCurlyBracket},
    {"}", reservedToken::closeCurlyBracket},
};

Token::Token(tokenValueType value) :
    _value(std::move(value))
    {
        
    }