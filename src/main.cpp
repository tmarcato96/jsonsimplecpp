#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <preproc_stream.hpp>
#include <tokenizer.hpp>

int main(int argc, char* argv[]) {
    std::string fileName = argv[1];
    std::ifstream inputFile(fileName);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);

        getcFunc lineStreamGetCharFunc = [&lineStream] {
            return lineStream.get();
        }; 

        preprocStream stream(lineStreamGetCharFunc);
        for (Token t = tokenize(stream); !t.isEOF(); t = tokenize(stream)) {
            if (t.isNumber()) {
                std::cout << "Number: " << t.getNumber() << std::endl;
            }
            else if (t.isString()) {
                std::cout << "String: " << t.getString() << std::endl;
            }
            else if (t.isSeparator()) {
                std::cout << "Separator: " << t.getSeparator() << std::endl;
            }
        }
    }
}