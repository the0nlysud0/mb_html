#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
namespace mb
{ namespace layout
{
  struct Token
  {
    std::string type;
    std::string value;
    std::map<std::string,std::string> attribs;
  };
  
  inline std::unordered_set<std::string> void_tags
  {
    "area", "base", "br", "col", "embed", "hr", "img",
    "input", "link", "meta", "source", "track", "wbr"
  };
  class Lexer
  {
  private:
    std::string input;
    int pos;
  public:
    Lexer(std::string html_file_path);
    // Lexer(const char* html_source);
    Token NextToken();
    char peek();
    char seek();  
  };
} // namespace layout
} // namespace mb
