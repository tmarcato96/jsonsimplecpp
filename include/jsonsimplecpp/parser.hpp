#pragma once

#include <fstream>

#include <jsonsimplecpp/tokenizer.hpp>
#include <jsonsimplecpp/node.hpp>

namespace Json {
    class JsonParser {
        std::ifstream _file;
        getcFunc _getcFunc;
        std::shared_ptr<JsonNode> _root;
        std::unique_ptr<JsonNode> _current;


        public:
            JsonParser(const std::string& filename);

        void parse();
        std::shared_ptr<JsonNode> parseObject(preprocStream& stream);
        std::shared_ptr<JsonNode> parseList(preprocStream& stream);
        std::shared_ptr<JsonNode> parseString(Token& token);
        std::shared_ptr<JsonNode> parseNumber(Token& token);

        std::shared_ptr<JsonNode> getJsonTree();

    };
}