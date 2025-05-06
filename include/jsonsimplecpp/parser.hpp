#pragma once

#include <fstream>

#include <jsonsimplecpp/node.hpp>
#include <jsonsimplecpp/tokenizer.hpp>

namespace Json {

  template<class VisitorPolicy = PrintVisitor> class JsonParser
  {
    std::ifstream _file;
    getcFunc _getcFunc;
    std::unique_ptr<JsonNode<VisitorPolicy>> _root;
    std::shared_ptr<VisitorPolicy> _visitor;

  public:
    explicit JsonParser(const std::string& filename);
    JsonParser(const std::string& filename, std::shared_ptr<VisitorPolicy> visitor);

    void parse();

    std::unique_ptr<JsonNode<VisitorPolicy>> parseObject(preprocStream& stream);
    std::unique_ptr<JsonNode<VisitorPolicy>> parseList(preprocStream& stream);
    std::unique_ptr<JsonNode<VisitorPolicy>> parseString(Token& token);
    std::unique_ptr<JsonNode<VisitorPolicy>> parseNumber(Token& token);

    // Non owning pointer
    const JsonNode<VisitorPolicy>* getJsonTree();
  };
} // namespace Json