#ifndef RAPL_EXCEPTION
#define RAPL_EXCEPTION
#include <exception>
#include <string>
class RAPLException : public std::exception
{
private:
  std::string message;

public:
  explicit RAPLException(const char *msg) : message(msg){};
  const char *what() const noexcept override
  {
    return message.c_str();
  }
};
#endif