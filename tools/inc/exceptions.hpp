#ifndef TOOL_EXCEPTION_GUARD
#define TOOL_EXCEPTION_GUARD

#include <exception>

namespace tools {
class TextureNotFoundException : public std::exception {
private:
  const char *message;

public:
  explicit TextureNotFoundException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class FontNotFoundException : public std::exception {
private:
  const char *message;

public:
  explicit FontNotFoundException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

} // namespace tools

#endif