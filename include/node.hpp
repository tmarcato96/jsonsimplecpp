#pragma once

#include <map>
#include <memory>
#include <string>
#include <optional>
#include <variant>

struct JsonNode;

using JsonObject = std::map<std::string, std::shared_ptr<JsonNode>>;
using JsonNodeType = std::variant<JsonObject*, std::string, double>;

struct JsonNode {

    JsonNodeType value;

    void print();
    std::optional<JsonObject::iterator> find(const std::string& key);

};

struct PrintVisitor {
    void operator() (JsonObject*);
    void operator() (const std::string&);
    void operator() (double);
};