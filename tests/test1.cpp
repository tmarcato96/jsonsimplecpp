#include <filesystem>
#include <fstream>
#include <iostream>

#include <jsonsimplecpp/parser.hpp>

struct Visitor
{
  void operator()(const std::unique_ptr<Json::JsonObject<Visitor>>&) {}
  void operator()(const std::unique_ptr<Json::JsonList<Visitor>>&) {}
  void operator()(const std::string&) {}
  void operator()(double) {}
};

int main()
{
#ifndef PROJECT_ROOT
#error "PROJECT_ROOT is not defined. Define it via CMake with target_compile_definitions."
#endif

  std::filesystem::path root_path = PROJECT_ROOT;
  std::filesystem::path outFilePath = root_path / "tests/output/out.json";
  std::filesystem::path inFilePath = root_path / "tests/input/test.json";

  std::ofstream outFile(outFilePath);

  Json::JsonParser parser(inFilePath);
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