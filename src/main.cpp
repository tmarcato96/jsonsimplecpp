#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <parser.hpp>

int main() {
    std::string fileName = "/src/src/data/test.json";
    JsonParser parser(fileName);
    parser.parse();
    parser._root->print();
    auto it = parser._root->find("primo");
    if (it.has_value()) {
        std::cout << "Found!\n";
        std::cout << it.value()->first << std::endl;
        if (std::holds_alternative<JsonList*>(it.value()->second->value)){
            auto list = std::get<JsonList*>(it.value()->second->value);
            for (auto l: *list) {
                std::cout << std::get<std::string>(l->value) << std::endl;
            }
        }
    }
    else {std::cout << "Not Found!\n";}
}