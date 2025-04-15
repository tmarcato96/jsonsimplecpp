#include <iostream>
#include <jsonsimplecpp/node.hpp>

using namespace Json;

PrintVisitor::PrintVisitor() :
  _out_stream(std::cout),
  _depth{0}
{}

PrintVisitor::PrintVisitor(std::ostream& out_stream) :
  _out_stream(out_stream),
  _depth{0}
{}

void PrintVisitor::operator()(const std::unique_ptr<JsonObject>& object)
{
  _out_stream << "{\n";
  ++_depth;
  for (auto it = object->begin(); it != object->end(); ++it) {
    // Indentation
    for (int i = 0; i < _depth; ++i) { _out_stream << '\t'; }

    _out_stream << '"' << it->first << '"' << ": ";
    std::visit(*this, it->second->value);
    auto next = it;
    ++next;
    if (next != object->end()) { _out_stream << ",\n"; }
  }
  _out_stream << '\n';
  --_depth;
  for (int i = 0; i < _depth; ++i) { _out_stream << '\t'; }
  _out_stream << '}';
}

void PrintVisitor::operator()(const std::unique_ptr<JsonList>& list)
{
  _out_stream << "[";
  for (auto it = list->begin(); it != list->end(); ++it) {
    std::visit(*this, (*it)->value);
    auto next = it;
    ++next;
    if (next != list->end()) { _out_stream << ", "; }
  }
  _out_stream << "]";
}

void PrintVisitor::operator()(const std::string& s) { _out_stream << '"' << s << '"'; }

void PrintVisitor::operator()(double num) { _out_stream << num; }

void JsonNode::print() const { std::visit(PrintVisitor{}, value); }

void JsonNode::print(std::ostream& out_stream) const { std::visit(PrintVisitor{out_stream}, value); }

std::optional<JsonObject::iterator> JsonNode::find(const std::string& key)
{
  if (std::holds_alternative<std::unique_ptr<JsonObject>>(value)) {
    JsonObject* object = std::get<std::unique_ptr<JsonObject>>(value).get();
    for (auto it = object->begin(); it != object->end(); ++it) {
      if (it->first == key) { return it; }
      if (std::holds_alternative<std::unique_ptr<JsonObject>>(it->second->value)) { return it->second->find(key); }
    }
    return std::nullopt;
  }
  else if (std::holds_alternative<std::unique_ptr<JsonList>>(value) || std::holds_alternative<std::string>(value) ||
           std::holds_alternative<double>(value)) {
    return std::nullopt;
  }
}