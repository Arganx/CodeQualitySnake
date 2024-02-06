#ifndef TOOL_EXCEPTION_GUARD
#define TOOL_EXCEPTION_GUARD

#include <exception>

namespace tools::exceptions {
class TextureNotFoundException : public std::exception {
private:
  const char *message;

public:
  explicit TextureNotFoundException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class TextureNotSetException : public std::exception {
private:
  const char *message;

public:
  explicit TextureNotSetException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class FontNotFoundException : public std::exception {
private:
  const char *message;

public:
  explicit FontNotFoundException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class ExpectedSizeMismatchException : public std::exception {
private:
  const char *message;

public:
  explicit ExpectedSizeMismatchException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class DatabaseException : public std::exception {
private:
  const char *message;

public:
  explicit DatabaseException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class IncorrectFormatException : public std::exception {
private:
  const char *message;

public:
  explicit IncorrectFormatException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

} // namespace tools::exceptions

#endif