#pragma once

#include <map>
#include <memory>
#include <string>
#include <variant>

struct JsonNode;

using JsonObject = std::map<std::string, std::shared_ptr<JsonNode>>;
using JsonNodeType = std::variant<JsonObject*, std::string, double>;

struct JsonNode {

    JsonNodeType value;

    void print();

};

struct PrintVisitor {
    void operator() (JsonObject*);
    void operator() (const std::string&);
    void operator() (double);
};