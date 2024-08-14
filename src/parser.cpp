#include <sstream>

#include <parser.hpp>
#include <preproc_stream.hpp>

using namespace Json;

JsonParser::JsonParser(const std::string& filename) :
    _file(filename) {
        _getcFunc = [this] {return this->_file.get();};
    }


void JsonParser::parse() {
    preprocStream stream(_getcFunc);
    for (Token t = tokenize(stream); !t.isEOF(); t = tokenize(stream)) {
        if (t.isSeparator()) {
            auto sep = t.getSeparator();
            if (sep == reservedToken::openCurlyBracket) {
                    std::shared_ptr<JsonNode> parsedObject = parseObject(stream);
                    if (!(_root)) {
                        _root = parsedObject;
                    }
            else if (sep == reservedToken::openSquareBracket) {
                std::shared_ptr<JsonNode> parsedList = parseList(stream);
                if (!(_root)) {
                    _root = parsedList;
                }
            }
        }
    }
    else if (t.isString()) {
        std::shared_ptr<JsonNode> parsedString = parseString(t);
        if (!_root) {
            _root = parsedString;
        }
    }
    else if (t.isNumber()) {
        std::shared_ptr<JsonNode> parsedNumber = parseNumber(t);
        if (!_root) {
            _root = parsedNumber;
        }
    }
    }
}

std::shared_ptr<JsonNode> JsonParser::parseObject(preprocStream& stream) {
    std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
    JsonObject* objectMap = new JsonObject();
    // Should we check if EOF?
    bool hasCompleted = false;
    while (!hasCompleted) {
        Token nextToken = tokenize(stream);
        std::string key;
        if (nextToken.isString()) {key = nextToken.getString();}
        else {throw std::runtime_error("Json Object should have a key.");}

        nextToken = tokenize(stream);
        if (!nextToken.isSeparator()) {throw std::runtime_error("Incorrect Json format.");}
        else {
            if (auto sep = nextToken.getSeparator(); sep != reservedToken::colon) {
                throw std::runtime_error("Incorrect Json format.");
            }
        }

        nextToken = tokenize(stream);
        if (nextToken.isString()) {
            (*objectMap)[key] = parseString(nextToken);
        }
        else if (nextToken.isNumber()) {
            (*objectMap)[key] = parseNumber(nextToken);
        }
        else if (nextToken.isSeparator()) {
            auto sep = nextToken.getSeparator();
            if (sep == reservedToken::openCurlyBracket) {
                (*objectMap)[key] = parseObject(stream);
            }
            else if (sep == reservedToken::openSquareBracket) {
                (*objectMap)[key] = parseList(stream);
            }
            else {throw std::runtime_error("Incorrect Json format.");}
        }
        else {throw std::runtime_error("Incorrect Json format.");}

        nextToken = tokenize(stream);
        if (!nextToken.isSeparator()) {throw std::runtime_error("Incorrect Json format.");}
        else {
            auto sep = nextToken.getSeparator();
            if (sep == reservedToken::comma) {
                continue;
            }
            else if (sep != reservedToken::closeCurlyBracket) {
                throw std::runtime_error("Incorrect Json format.");
            }
            hasCompleted = true;
        }
    }
    
    node->value = objectMap;
    return node;

}

std::shared_ptr<JsonNode> JsonParser::parseList(preprocStream& stream) {
    std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
    JsonList* list = new JsonList();

    bool hasCompleted = false;
    while (!hasCompleted) {
        Token nextToken = tokenize(stream);
        std::shared_ptr<JsonNode> node;
        if (nextToken.isSeparator()) {
            auto sep = nextToken.getSeparator();
            if (sep == reservedToken::openSquareBracket) {node = parseList(stream);}
            else if (sep == reservedToken::openCurlyBracket) {node = parseObject(stream);}
            else {throw std::runtime_error("Incorrect Json format.");}
        }
        else if (nextToken.isString()) {
            node = parseString(nextToken);
        }
        else if (nextToken.isNumber()) {
            node = parseNumber(nextToken);
        }
        list->push_back(node);
        nextToken = tokenize(stream);
        if (!nextToken.isSeparator()) {throw std::runtime_error("Incorrect Json format.");}
        else {
            auto sep = nextToken.getSeparator();
            if (sep == reservedToken::comma) {continue;}
            else if (sep != reservedToken::closeSquareBracket) {
                throw std::runtime_error("Incorrect Json format.");
            }
            hasCompleted = true;
        }
    }
    node->value = list;
    return node;
}

std::shared_ptr<JsonNode> JsonParser::parseString(Token& token) {
    std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
    node->value = token.getString();
    return node;
}

std::shared_ptr<JsonNode> JsonParser::parseNumber(Token& token) {
    std::shared_ptr<JsonNode> node = std::make_shared<JsonNode>();
    node->value = token.getNumber();
    return node;
}