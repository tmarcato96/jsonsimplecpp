#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

#include <tokenizer.hpp>
#include <preproc_stream.hpp>

enum struct CharType {
    space,
    eof,
    punctuation,
    alphanumeric,
};

CharType getCharType(int c) {
    if (c < 0) return CharType::eof;

    if (std::isspace(c)) return CharType::space;

    if (std::isalnum(c) || c=='_') return CharType::alphanumeric;

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

Token tokenizer(preprocStream& stream){
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
                }
        }


        break;
    }
}