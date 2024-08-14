#include <iostream>
#include <node.hpp>

using namespace Json;

void PrintVisitor::operator()(JsonObject* object) {
    std::cout << "{\n";
    for (auto it = object->begin(); it != object->end(); ++it) {
        std::cout << it->first << ": ";
        std::visit(PrintVisitor{}, it->second->value);
        auto next = it;
        ++next;
        if (next != object->end()) {std::cout << ",\n";}
        std::cout << std::endl;
    }
    std::cout << "}\n";

}

void PrintVisitor::operator()(JsonList* list) {
    std::cout << "[\n";
    for (auto it = list->begin(); it != list->end(); ++it) {
        std::visit(PrintVisitor{}, (*it)->value);
        auto next = it;
        ++next;
        if (next != list->end()) {std::cout << ",\n";}
        std::cout << std::endl;
    }
    std::cout << "]\n";
}

void PrintVisitor::operator()(const std::string& s) {
    std::cout << s;
}

void PrintVisitor::operator()(double num) {
    std::cout << num;
}

void JsonNode::print() {
    std::visit(PrintVisitor{}, value);
}

std::optional<JsonObject::iterator> JsonNode::find(const std::string& key) {
    if (std::holds_alternative<JsonObject*>(value)) {
        JsonObject* object = std::get<JsonObject*>(value);
        for (auto it = object->begin(); it != object->end(); ++it) {
            if (it->first == key) {
                return it;
            }
            if (std::holds_alternative<JsonObject*>(it->second->value)) {
                return it->second->find(key);
            }
        }
        return std::nullopt;
    }
    else if (std::holds_alternative<JsonList*>(value) || std::holds_alternative<std::string>(value) || std::holds_alternative<double>(value)) {
        return std::nullopt;
    }
}