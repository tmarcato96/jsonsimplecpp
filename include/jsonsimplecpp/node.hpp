#pragma once

#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  struct JsonNode;

  using JsonObject = std::map<std::string, std::shared_ptr<JsonNode>>;
  using JsonList = std::vector<std::shared_ptr<JsonNode>>;
  using JsonNodeType = std::variant<JsonObject*, JsonList*, std::string, double>;

  struct JsonNode
  {
    JsonNodeType value;

    void print();
    void print(std::ostream&);
    std::optional<JsonObject::iterator> find(const std::string& key);
  };

  struct PrintVisitor
  {
    void operator()(JsonObject*);
    void operator()(JsonList*);
    void operator()(const std::string&);
    void operator()(double);

    PrintVisitor();
    PrintVisitor(std::ostream&);

  private:
    std::ostream& _out_stream;
    size_t _depth;
  };
} // namespace Json