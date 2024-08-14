#include <cctype>
#include <map>
#include <string_view>
#include <stdexcept>
#include <string>
#include <utility>

#include <jsonsimplecpp/tokenizer.hpp>
#include <jsonsimplecpp/preproc_stream.hpp>

namespace Json {
    enum struct CharType {
        space,
        eof,
        punctuation,
        numeric,
    };

    const std::map<std::string_view, reservedToken> specialCharacterMap {
        {":", reservedToken::colon},
        {",", reservedToken::comma},
        
        {"[", reservedToken::openSquareBracket},
        {"]", reservedToken::closeSquareBracket},

        {"{", reservedToken::openCurlyBracket},
        {"}", reservedToken::closeCurlyBracket},
    };

    const std::map<reservedToken, std::string_view> reversedCharacterMap = ([](){
        std::map<reservedToken, std::string_view> ret;
        for (const auto& element: specialCharacterMap) {
            ret.emplace(element.second, element.first);
        }
        return ret;
    })();

    std::ostream& operator<<(std::ostream& os, reservedToken t) {
        os << reversedCharacterMap.find(t)->second;
        return os;
    }

    CharType getCharType(int c) {
        if (c < 0) return CharType::eof;

        if (std::isspace(c)) return CharType::space;

        if (std::isdigit(c)) return CharType::numeric;

        return CharType::punctuation; // Only other possibilities are control codes
    }

    Token getString(preprocStream& stream) {
        std::string str;
        int c = stream.getc();

        for (; getCharType(c) != CharType::eof; c = stream.getc()) {
            
            switch (c) {
                case '"':
                    return Token(std::move(str));
                default:
                    str.push_back(c);
            }
        }
        throw std::runtime_error("Invalid file format!");
    }

    Token getNumber(preprocStream& stream) {
        std::string str;
        int c = stream.getc();

        for(; std::isdigit(c) || c == '.'; c = stream.getc()) {
            str.push_back(c);
        }
        stream.push_back(c);
        return Token(std::stod(str));
    }

    Token getSeparator(preprocStream& stream) {

        std::string str;
        str.push_back(stream.getc());
        auto it = specialCharacterMap.find(str);

        if (it != specialCharacterMap.end()) {
            // The character is one of the reserved tokens
            return Token(it->second);
        }
        else throw std::runtime_error("Unexpected character!");

    }

    Token tokenize(preprocStream& stream){
        while (true) {
            int c = stream.getc();
            switch (getCharType(c)) {

                case CharType::eof:
                    return {eof()};

                case CharType::space:
                    continue;

                case CharType::punctuation:
                    switch (c) {
                        case '"':
                            return getString(stream);

                        default:
                            stream.push_back(c);
                            return getSeparator(stream);
                    }
                
                case CharType::numeric:
                    stream.push_back(c);
                    return getNumber(stream);
            }
            break;
        }
    }
}