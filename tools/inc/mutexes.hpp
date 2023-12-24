#ifndef MUTEXES_GUARD
#define MUTEXES_GUARD

#include <mutex>
namespace tools {
struct Mutexes {
  std::mutex snakeBlockMutex{};
  std::mutex candyBlocksMutex{};
  std::mutex directionMutex{};
};
} // namespace tools
#endif