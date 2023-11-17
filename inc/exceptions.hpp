#ifndef EXCEPTION_GUARD
#define EXCEPTION_GUARD

#include <exception>

namespace Game::SnakeExceptions {

class NonAdjacentMovementException : public std::exception {
private:
  const char *message;

public:
  explicit NonAdjacentMovementException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class MovingIntoItselfException : public std::exception {
private:
  const char *message;

public:
  explicit MovingIntoItselfException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class SegmentNonAdjacentException : public std::exception {
private:
  const char *message;

public:
  explicit SegmentNonAdjacentException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class PointerNotInitializedException : public std::exception {
private:
  const char *message;

public:
  explicit PointerNotInitializedException(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};

class UnexpectedSwitchValue : public std::exception {
private:
  const char *message;

public:
  explicit UnexpectedSwitchValue(const char *msg) : message(msg) {}
  const char *what() const noexcept override { return message; }
};
} // namespace Game::SnakeExceptions
#endif