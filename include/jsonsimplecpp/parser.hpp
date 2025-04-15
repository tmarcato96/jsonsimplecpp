#pragma once

#include <fstream>

#include <jsonsimplecpp/node.hpp>
#include <jsonsimplecpp/tokenizer.hpp>

namespace Json {
  class JsonParser
  {
    std::ifstream _file;
    getcFunc _getcFunc;
    std::unique_ptr<JsonNode> _root;

  public:
    JsonParser(const std::string& filename);

    void parse();
    std::unique_ptr<JsonNode> parseObject(preprocStream& stream);
    std::unique_ptr<JsonNode> parseList(preprocStream& stream);
    std::unique_ptr<JsonNode> parseString(Token& token);
    std::unique_ptr<JsonNode> parseNumber(Token& token);

    // Non owning pointer
    const JsonNode* getJsonTree();
  };
} // namespace Json