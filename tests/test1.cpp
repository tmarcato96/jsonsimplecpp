#include <fstream>
#include <iostream>

#include <jsonsimplecpp/parser.hpp>

int main()
{
  std::ofstream outFile("./tests/output/out.json");

  Json::JsonParser parser("./tests/input/test.json");
  parser.parse();
  auto jsonTree = parser.getJsonTree();
  if (outFile.is_open()) {
    jsonTree->print(outFile);
    outFile.close();
  }
  else {
    std::cout << "Unable to open file!\n";
  }
}