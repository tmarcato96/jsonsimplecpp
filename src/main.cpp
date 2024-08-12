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
}