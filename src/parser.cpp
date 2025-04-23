#include <sstream>

#include <jsonsimplecpp/parser.hpp>
#include <jsonsimplecpp/preproc_stream.hpp>

using namespace Json;

template<class VisitorPolicy>
JsonParser<VisitorPolicy>::JsonParser(const std::string& filename) :
  _file(filename),
  _visitor(std::make_shared<VisitorPolicy>())
{
  _getcFunc = [this] { return this->_file.get(); };
}

template<class VisitorPolicy> void JsonParser<VisitorPolicy>::parse()
{
  preprocStream stream(_getcFunc);
  for (Token t = tokenize(stream); !t.isEOF(); t = tokenize(stream)) {
    if (t.isSeparator()) {
      auto sep = t.getSeparator();
      if (sep == reservedToken::openCurlyBracket) {
        auto parsedObject = parseObject(stream);
        if (!(_root)) { _root = std::move(parsedObject); }
        else if (sep == reservedToken::openSquareBracket) {
          auto parsedList = parseList(stream);
          if (!(_root)) { _root = std::move(parsedList); }
        }
      }
    }
    else if (t.isString()) {
      auto parsedString = parseString(t);
      if (!_root) { _root = std::move(parsedString); }
    }
    else if (t.isNumber()) {
      auto parsedNumber = parseNumber(t);
      if (!_root) { _root = std::move(parsedNumber); }
    }
  }
}

template<class VisitorPolicy>
std::unique_ptr<JsonNode<VisitorPolicy>> JsonParser<VisitorPolicy>::parseObject(preprocStream& stream)
{
  using Node = JsonNode<VisitorPolicy>;
  using Object = JsonObject<VisitorPolicy>;

  auto node = std::make_unique<Node>();
  auto objectMap = std::make_unique<Object>();
  node->setVisitor(_visitor);

  // Should we check if EOF?
  bool hasCompleted = false;
  while (!hasCompleted) {
    Token nextToken = tokenize(stream);
    std::string key;
    if (nextToken.isString()) { key = nextToken.getString(); }
    else {
      throw std::runtime_error("Json Object should have a key.");
    }

    nextToken = tokenize(stream);
    if (!nextToken.isSeparator()) { throw std::runtime_error("Incorrect Json format."); }
    else {
      if (auto sep = nextToken.getSeparator(); sep != reservedToken::colon) {
        throw std::runtime_error("Incorrect Json format.");
      }
    }

    nextToken = tokenize(stream);
    if (nextToken.isString()) { (*objectMap)[key] = parseString(nextToken); }
    else if (nextToken.isNumber()) {
      (*objectMap)[key] = parseNumber(nextToken);
    }
    else if (nextToken.isSeparator()) {
      auto sep = nextToken.getSeparator();
      if (sep == reservedToken::openCurlyBracket) { (*objectMap)[key] = parseObject(stream); }
      else if (sep == reservedToken::openSquareBracket) {
        (*objectMap)[key] = parseList(stream);
      }
      else {
        throw std::runtime_error("Incorrect Json format.");
      }
    }
    else {
      throw std::runtime_error("Incorrect Json format.");
    }

    nextToken = tokenize(stream);
    if (!nextToken.isSeparator()) { throw std::runtime_error("Incorrect Json format."); }
    else {
      auto sep = nextToken.getSeparator();
      if (sep == reservedToken::comma) { continue; }
      else if (sep != reservedToken::closeCurlyBracket) {
        throw std::runtime_error("Incorrect Json format.");
      }
      hasCompleted = true;
    }
    (*objectMap)[key]->setVisitor(_visitor);
  }

  node->value = std::move(objectMap);
  return node;
}

template<class VisitorPolicy>
std::unique_ptr<JsonNode<VisitorPolicy>> JsonParser<VisitorPolicy>::parseList(preprocStream& stream)
{
  using Node = JsonNode<VisitorPolicy>;
  using List = JsonList<VisitorPolicy>;

  auto node = std::make_unique<Node>();
  auto list = std::make_unique<List>();
  node->setVisitor(_visitor);

  bool hasCompleted = false;
  while (!hasCompleted) {
    Token nextToken = tokenize(stream);
    std::unique_ptr<Node> child;
    if (nextToken.isSeparator()) {
      auto sep = nextToken.getSeparator();
      if (sep == reservedToken::openSquareBracket) { child = parseList(stream); }
      else if (sep == reservedToken::openCurlyBracket) {
        child = parseObject(stream);
      }
      else {
        throw std::runtime_error("Incorrect Json format.");
      }
    }
    else if (nextToken.isString()) {
      child = parseString(nextToken);
    }
    else if (nextToken.isNumber()) {
      child = parseNumber(nextToken);
    }
    child->setVisitor(_visitor);
    list->push_back(std::move(child));

    nextToken = tokenize(stream);
    if (!nextToken.isSeparator()) { throw std::runtime_error("Incorrect Json format."); }
    else {
      auto sep = nextToken.getSeparator();
      if (sep == reservedToken::comma) { continue; }
      else if (sep != reservedToken::closeSquareBracket) {
        throw std::runtime_error("Incorrect Json format.");
      }
      hasCompleted = true;
    }
  }

  node->value = std::move(list);
  return node;
}

template<class VisitorPolicy>
std::unique_ptr<JsonNode<VisitorPolicy>> JsonParser<VisitorPolicy>::parseString(Token& token)
{
  auto node = std::make_unique<JsonNode<VisitorPolicy>>();
  node->value = token.getString();
  node->setVisitor(_visitor);
  return node;
}

template<class VisitorPolicy>
std::unique_ptr<JsonNode<VisitorPolicy>> JsonParser<VisitorPolicy>::parseNumber(Token& token)
{
  auto node = std::make_unique<JsonNode<VisitorPolicy>>();
  node->value = token.getNumber();
  node->setVisitor(_visitor);
  return node;
}

template<class VisitorPolicy> const JsonNode<VisitorPolicy>* JsonParser<VisitorPolicy>::getJsonTree()
{
  if (!_root) { parse(); }
  return _root.get();
}

template class JsonParser<PrintVisitor>;