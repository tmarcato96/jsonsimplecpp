#include <iostream>
#include <node.hpp>

void PrintVisitor::operator()(JsonObject* object) {
    std::cout << "{\n";
    for (auto it = object->begin(); it != object->end(); ++it) {
        std::cout << it->first << ": ";
        std::visit(PrintVisitor{}, it->second->value);
        auto next = it;
        ++next;
        if (next != object->end()) {std::cout << " ,\n";}
        std::cout << std::endl;
    }
    std::cout << "}\n";

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