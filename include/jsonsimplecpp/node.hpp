#pragma once

#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  struct PrintVisitor;

  template<class VisitorPolicy> class JsonNode;

  template<class VisitorPolicy> using JsonObject = std::map<std::string, std::unique_ptr<JsonNode<VisitorPolicy>>>;

  template<class VisitorPolicy> using JsonList = std::vector<std::unique_ptr<JsonNode<VisitorPolicy>>>;

  template<class VisitorPolicy = PrintVisitor> class JsonNode
  {
  public:
    using Visitor = VisitorPolicy;
    using Object = JsonObject<Visitor>;
    using List = JsonList<Visitor>;
    using NodeType = std::variant<std::unique_ptr<Object>, std::unique_ptr<List>, std::string, double>;

    NodeType value;

    JsonNode() = default;
    explicit JsonNode(NodeType val, std::shared_ptr<Visitor> visitor = nullptr) :
      value{std::move(val)},
      _visitor{visitor}
    {}

    void setVisitor(std::shared_ptr<Visitor> visitor) { _visitor = visitor; }

    void traverse() const { std::visit(*_visitor, value); }

    void print() const { traverse(); }

    void print(std::ostream& os) const
    {
      _visitor->setStream(os);
      traverse();
    }

    std::optional<typename Object::iterator> find(const std::string& key)
    {
      if (std::holds_alternative<std::unique_ptr<Object>>(value)) {
        Object* object = std::get<std::unique_ptr<Object>>(value).get();
        for (auto it = object->begin(); it != object->end(); ++it) {
          if (it->first == key) { return it; }
          if (std::holds_alternative<std::unique_ptr<Object>>(it->second->value)) {
            auto nested = it->second->find(key);
            if (nested) return nested;
          }
        }
      }
      return std::nullopt;
    }

    // Utilities
    bool isObject() const { return std::holds_alternative<std::unique_ptr<Object>>(value); }
    bool isList() const { return std::holds_alternative<std::unique_ptr<List>>(value); }
    bool isString() const { return std::holds_alternative<std::string>(value); }
    bool isNumber() const { return std::holds_alternative<double>(value); }

  private:
    std::shared_ptr<Visitor> _visitor = nullptr;
  }; // namespace Json

  struct PrintVisitor
  {
    void operator()(const std::unique_ptr<JsonObject<PrintVisitor>>&);
    void operator()(const std::unique_ptr<JsonList<PrintVisitor>>&);
    void operator()(const std::string&);
    void operator()(double);

    PrintVisitor();
    PrintVisitor(std::ostream&);

    void setStream(std::ostream& stream);

  private:
    std::ostream* _out_stream;
    size_t _depth;
  };
} // namespace Json