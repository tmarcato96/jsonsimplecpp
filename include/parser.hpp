#pragma once

#include <fstream>

#include <tokenizer.hpp>
#include <node.hpp>

class JsonParser {
    std::ifstream _file;
    getcFunc _getcFunc;


    public:
        JsonParser(const std::string& filename);

    void parse();
    std::shared_ptr<JsonNode> parseObject(preprocStream& stream);
    std::shared_ptr<JsonNode> parseString(Token& token);
    std::shared_ptr<JsonNode> parseNumber(Token& token);

    std::shared_ptr<JsonNode> _root;
    std::unique_ptr<JsonNode> _current;
};