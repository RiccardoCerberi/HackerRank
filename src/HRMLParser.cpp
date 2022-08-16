#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Attribute {
  friend class Tag;
public:
  Attribute(string const&);
  Attribute(string const&, string const&);
  void AddContent(string const&);
  ostream& ShowContent(ostream&) const;
private:
  string m_attributeName;
  string m_content;
};

Attribute::Attribute(string const& attributeName) : m_attributeName(attributeName) {}

Attribute::Attribute(string const& attributeName, string const& content) : m_attributeName(attributeName), m_content(content) {}

// the content passed in the constructor have "" so it's needed arithmetical operation
void Attribute::AddContent(string const& content) {
  auto lastCharIterator = content.end() - 1;
  if (*lastCharIterator == '>')
    --lastCharIterator; // the last iterator has to move two positions back to avoid including \"
  m_content = string(content.begin() + 1, lastCharIterator);
}

ostream& Attribute::ShowContent(ostream& out) const {
  out << m_content;
  return out;
}

class Tag : public enable_shared_from_this<Tag> {
  friend class Parser;
public:
  Tag(string const& tagName, shared_ptr<Tag> const& outerTag)
      : m_tagName(MakeTagNameWithoutSpecialChars(tagName)), m_outerTag(outerTag) {
  }
  void AddAttribute(string const&);
  shared_ptr<Tag> AddNestedTag(string const&);

  shared_ptr<Tag> FindNestedTag(string const&);  // change it in case of nullptr to avoid segmentation fault
  Attribute FindAttribute(string const&);        // change return value to attribute and control possible shortcuts
private:
  string MakeTagNameWithoutSpecialChars(string const&);
  string m_tagName;
  weak_ptr<Tag> m_outerTag;              // weak to avoid cyclic reference
  vector<shared_ptr<Tag>> m_nestedTags;  // shared in fact what is pointed could be shared by m_tagToControll Parse's method
  vector<Attribute> m_attributes;
};

string Tag::MakeTagNameWithoutSpecialChars(string const& tagName) {
  auto firstChar = tagName.begin() + 1; // to remove <
  auto lastChar = tagName.end() - 1;
  if (*lastChar != '>')  // to check if it's <tag1> or <tag1 n = "e">
    ++lastChar;
  return string(firstChar, lastChar);
}

void Tag::AddAttribute(string const& attributeName) {
  m_attributes.emplace_back(attributeName);
}

shared_ptr<Tag> Tag::AddNestedTag(string const& tagName) {
  shared_ptr<Tag> temp = make_shared<Tag>(tagName, shared_from_this());
  m_nestedTags.push_back(temp);
  return temp;
}

shared_ptr<Tag> Tag::FindNestedTag(string const& nestedTagName) {
  auto limitForNestedTag = m_nestedTags.end();
  auto nestedTagIterator = find_if(m_nestedTags.begin(), limitForNestedTag, [&nestedTagName](shared_ptr<Tag>& tagPtr) {
    return nestedTagName == tagPtr->m_tagName;
  });
  if (nestedTagIterator == limitForNestedTag)
    return nullptr;
  return *nestedTagIterator;
}

Attribute Tag::FindAttribute(string const& attributeName) {
  auto AttributeIterator = find_if(m_attributes.begin(), m_attributes.end(), [&attributeName](Attribute& attributeRef) {
    return attributeName == attributeRef.m_attributeName;
  });

  if (AttributeIterator == m_attributes.end()) {
    return Attribute("error", "Not Found!");
  }
  return *AttributeIterator;
}

class Parser {
public:
  Parser();
  //~Parser();
  void PrintResult();
private:
  void GenerateStructure();
  void GetLineNumbers(string const&);
  void ComputeWord();
  bool IsTag();
  bool IsPreviousTag();
  bool IsPreviousContent();
  bool IsAttribute();
  bool IsAttributeContent();
  bool IsEndTag();

  void CreateNewNestedTag();
  void AddAttributeToTag();
  void FillAttributeContent();
  void ReturnToOuterTag();

  void ComputeQuery();
  bool ComputeSpecialCharacter(char);
  shared_ptr<Tag> SearchTag();
  Attribute SearchAttribute();

  struct TagsLevel0 {
    
    vector<shared_ptr<Tag>> m_tagsLevel0;
    shared_ptr<Tag> searchTagLevel0(string const&);
  } m_tagsLevel0Root;

  shared_ptr<Tag> m_tagToControll;  
  string m_currentWord;
  string m_previousWord;
  int m_totSourceLines;
  int m_totQueries;
};

Parser::Parser() : m_tagToControll{nullptr}, m_totSourceLines{0}, m_totQueries{0} {}

void Parser::PrintResult() {
  // cin doesn't take new line that would be catched by getline, therefore it's necessary to use getline at first
  string numberOfLines;

  getline(cin, numberOfLines);

  GetLineNumbers(numberOfLines);
  GenerateStructure();
  ComputeQuery();
}

void Parser::GetLineNumbers(string const& line) {
  istringstream record(line);
  string strNumber;

  record >> strNumber;
  m_totSourceLines = stoi(strNumber);
  record >> strNumber;
  m_totQueries = stoi(strNumber);
}

void Parser::GenerateStructure() {
  int currentSourceLine = 0;
  string currentLine;

  while (currentSourceLine < m_totSourceLines) {
    getline(cin, currentLine);

    istringstream record(currentLine);
    while (record >> m_currentWord) {
      ComputeWord();
      m_previousWord = m_currentWord;
    }
    ++currentSourceLine;
  }
}

// functions to generate the structure
void Parser::ComputeWord() {
  if (IsTag())
    CreateNewNestedTag();
  else if (IsAttributeContent())
    FillAttributeContent();
  else if (IsEndTag())
    ReturnToOuterTag();
  else if (IsAttribute())
    AddAttributeToTag();
}

bool Parser::IsTag() {
  return m_currentWord.length() >= 2 && m_currentWord[0] == '<' && m_currentWord[1] != '/';
}

bool Parser::IsAttributeContent() {
  return m_currentWord[0] == '"';  // check if it's correct the syntax
}

bool Parser::IsPreviousTag() {
  auto temp = m_currentWord;
  m_currentWord = m_previousWord; // to use IsTag pretend the currentWord is the previous one
  bool isTag =  IsTag();
  m_currentWord = temp;
  return isTag;
}

bool Parser::IsPreviousContent() {
  auto temp = m_currentWord;
  m_currentWord = m_previousWord; // to use IsPreviousContent pretend the currentWord is the previous one
  bool isTag =  IsAttributeContent();
  m_currentWord = temp;
  return isTag;
}


bool Parser::IsAttribute() {
  return IsPreviousTag() == true || IsPreviousContent() == true;
}



bool Parser::IsEndTag() {
  return m_currentWord.length() >= 2 && m_currentWord[0] == '<' && m_currentWord[1] == '/';
}

void Parser::CreateNewNestedTag() {
  if (m_tagToControll == nullptr) {
    // first scope: tags have no outer tag
    auto newTag = make_shared<Tag>(m_currentWord, nullptr);
    m_tagsLevel0Root.m_tagsLevel0.push_back(newTag);
    m_tagToControll = newTag;
  } else {
    m_tagToControll = m_tagToControll->AddNestedTag(m_currentWord);
  }
}

void Parser::AddAttributeToTag() {
  // m_tagToContoroll cannot be null, it would mean that an attribute is declared before a tag that is impossible for source code lines correctness
  assert(m_tagToControll != nullptr);
  m_tagToControll->AddAttribute(m_currentWord);
}

void Parser::FillAttributeContent() {
  auto lastAttributeInserted = m_tagToControll->m_attributes.size() - 1;  // it cannot be negative because the problem ensures correctness in the source code
  assert(lastAttributeInserted >= 0);
  m_tagToControll->m_attributes[lastAttributeInserted].AddContent(m_currentWord);
}

void Parser::ReturnToOuterTag() {
  assert(m_tagToControll != nullptr);
  m_tagToControll = (m_tagToControll->m_outerTag).lock();
}

// functions to compute queries

void Parser::ComputeQuery() {
  string currentQuery;

  // if it returns a nullptr, change the code to print out an invalid attribute:
  // use an attribute result that in case of no tag match containes Not Found! Again be aware of copy construction and copy assignment operator
  while (m_totQueries > 0 && getline(cin, currentQuery)) {
    bool nestedTagFound = false;
    m_currentWord = "";
    m_tagToControll = nullptr;  // for every query  the starter scope must be 0
    for (auto c : currentQuery) {
      if (ComputeSpecialCharacter(c)) {
        if (m_tagToControll == nullptr) {
          nestedTagFound = false;
          break;
        } else {
          nestedTagFound = true;
        }
        m_currentWord = "";
      } else {
        m_currentWord += c;
      }
    }
    if (nestedTagFound == false) {
      Attribute("error", "Not Found!").ShowContent(cout) << endl;
    } else {
      SearchAttribute().ShowContent(cout) << endl;
    }
    --m_totQueries;
}
}

bool Parser::ComputeSpecialCharacter(char c) {
  bool isSpecial = true;
  if (c == '.' || c == '~') {
    m_tagToControll = SearchTag();
  } else {
    isSpecial = false;
  }
  return isSpecial;
}

shared_ptr<Tag> Parser::TagsLevel0::searchTagLevel0(string const& tagLevel0Name) {
  auto lastLevel0Tag = m_tagsLevel0.end();
  auto level0TagIterator = find_if(m_tagsLevel0.begin(), lastLevel0Tag, [&tagLevel0Name](shared_ptr<Tag>& tagPtr) {
    return tagLevel0Name == tagPtr->m_tagName;
  });
  if (level0TagIterator == lastLevel0Tag)
    return nullptr;  // if a tag is not found, nullptr will be returned
  return *level0TagIterator;
}

shared_ptr<Tag> Parser::SearchTag() {
  if (m_tagToControll == nullptr) {
    return m_tagsLevel0Root.searchTagLevel0(m_currentWord);
  }
  return m_tagToControll->FindNestedTag(m_currentWord);
}

Attribute Parser::SearchAttribute() {
  return m_tagToControll->FindAttribute(m_currentWord);
}
