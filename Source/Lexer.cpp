#include "Lexer.h"
#include <sstream>
#include <fstream>
namespace mb::layout
{
  Lexer::Lexer(std::string html_file_path)
  {
  // read the file
  std::ifstream html_file(html_file_path);
  if(!html_file.is_open())
  {
    std::cout << "failed to load html file ," << html_file_path << '\n';
    return;
  }

  std::ostringstream ss;
  ss << html_file.rdbuf();
  input = ss.str();
  pos = 0;
  }

  // Lexer::Lexer(const char *html_source)
  // {
  //   input = html_source;
  //   pos = 0;
  // }

  Token Lexer::NextToken()
  {
    while (isspace(peek())) seek();

    if (peek() == '\0') return {"eof", ""};
    bool is_closing = false;
    if(peek()=='<')
    {
      seek();
      bool is_closing = false;
      if(peek() == '/')
      {
        seek();
        is_closing = true;
      }
      std::string tag_name;
      while (isalnum(peek())) tag_name += seek();
      // reading attributes
      std::map<std::string , std::string> attributes;
      while(peek() != '>' && peek() != '/' && peek() !='\0')
      {
        while(isspace(peek())) seek();
        std::string attr_name;
        while (isalnum(peek()) || peek() == '-' || peek() == ':') attr_name += seek();

        std::string attrib_value;
        if(peek() == '=')
        {
          seek();

          // check for quoted values
           if (peek() == '"' || peek() == '\'') {
            char quote = seek();
            while (peek() != quote && peek() != '\0') attrib_value += seek();
            seek(); // consume closing quote
          } else {
            // unquoted value
            while (!isspace(peek()) && peek() != '>' && peek() != '/') attrib_value += seek();
          }
        }

        if (!attr_name.empty()) attributes[attr_name] = attrib_value;
      }

      bool self_closing = false;
      if (peek() == '/') {
        self_closing = true;
        seek(); // consume '/'
      }

      seek(); // consume '>'
      if(self_closing) return {"self-close" , tag_name , attributes};
      if(is_closing) return {"tag-close" , tag_name};
      if (void_tags.count(tag_name)) return {"self-close", tag_name , attributes};
      return {"tag-open" , tag_name , attributes};
    }

    // Text node
    std::string text;
    while (peek() != '<' && peek() != '\0') text += seek();

    // Trim leading/trailing whitespace
    size_t start = text.find_first_not_of(" \t\n\r");
    size_t end = text.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) {
      // text is only whitespace, skip it and get next token
      return NextToken();
    }
    text = text.substr(start, end - start + 1);

    return {"#text", text};
  }

  char Lexer::peek()
  {
    if(pos >= input.length()) return '\0';
    return input[pos];
  }

  char Lexer::seek()
  {
    if (pos >= input.size()) return '\0';
    return input[pos++];
  }

} // namespace mb::layout