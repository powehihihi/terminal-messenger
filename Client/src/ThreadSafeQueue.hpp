#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <queue>
#include <mutex>

namespace ThreadSafe {
  template<typename T>
  class Queue {
   private:
    std::queue<T> q;
    std::mutex m;
   public:
    void push(const T& item) {
      std::unique_lock<std::mutex> lk(m);
      q.push(item);
    }
    bool pop(T& item) {
      std::unique_lock<std::mutex> lk(m);
      if (q.empty())
        return false;
      item = q.front();
      q.pop();
      return true;
    }
  };
}
#endif // !THREAD_SAFE_QUEUE

