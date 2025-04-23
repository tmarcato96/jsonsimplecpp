#include <iostream>
#include <jsonsimplecpp/node.hpp>

using namespace Json;

PrintVisitor::PrintVisitor() :
  _out_stream(&std::cout),
  _depth{0}
{}

PrintVisitor::PrintVisitor(std::ostream& out_stream) :
  _out_stream(&out_stream),
  _depth{0}
{}

void PrintVisitor::setStream(std::ostream& stream) { _out_stream = &stream; }

void PrintVisitor::operator()(const std::unique_ptr<JsonObject<PrintVisitor>>& object)
{
  *_out_stream << "{\n";
  ++_depth;
  for (auto it = object->begin(); it != object->end(); ++it) {
    // Indentation
    for (int i = 0; i < _depth; ++i) { *_out_stream << '\t'; }

    *_out_stream << '"' << it->first << '"' << ": ";
    std::visit(*this, it->second->value);
    if (std::next(it) != object->end()) { *_out_stream << ",\n"; }
  }
  *_out_stream << '\n';
  --_depth;
  for (int i = 0; i < _depth; ++i) { *_out_stream << '\t'; }
  *_out_stream << '}';
}

void PrintVisitor::operator()(const std::unique_ptr<JsonList<PrintVisitor>>& list)
{
  *_out_stream << "[";
  for (auto it = list->begin(); it != list->end(); ++it) {
    std::visit(*this, (*it)->value);
    auto next = it;
    ++next;
    if (next != list->end()) { *_out_stream << ", "; }
  }
  *_out_stream << "]";
}

void PrintVisitor::operator()(const std::string& s) { *_out_stream << '"' << s << '"'; }

void PrintVisitor::operator()(double num) { *_out_stream << num; }

template<class VisitorPolicy> void JsonNode<VisitorPolicy>::print() const { traverse(); }

template<class VisitorPolicy> void JsonNode<VisitorPolicy>::print(std::ostream& os) const
{
  _visitor->setStream(os);
  traverse();
}

template class JsonNode<PrintVisitor>;