#pragma once

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

#include "./task.hpp"

// FIXME: Moving a `std::packaged_task` does not work in MSVC.
//
// See https://stackoverflow.com/a/16152351/3645337,
// https://stackoverflow.com/a/48999075/3645337, and the king
// himself at
// https://developercommunity.visualstudio.com/content/problem/108672/unable-to-move-stdpackaged-task-into-any-stl-conta.html
//

/// A thread pool with prioritize-able tasks.
///
/// @note It is broken on MSVC.
class ThreadPool {
public:
  ThreadPool(size_t threads);
  ~ThreadPool();

  /// Enqueue a task with explicit priority.
  template <class F, class... A>
  decltype(auto) enqueue(int priority, F &&f, A &&... a);

  /// Enqueue a task with implicit priority of zero.
  template <class F, class... A> decltype(auto) enqueue(F &&f, A &&... a);

private:
  using TaskType = std::unique_ptr<ITask>;

  struct TaskEntry {
    int priority;
    mutable TaskType task;

    TaskEntry(int priority, TaskType task);

    bool operator<(const TaskEntry &i) const { return priority < i.priority; }
  };

  bool _stopped = false;
  std::mutex _mutex;
  std::condition_variable _condvar;
  std::vector<std::thread> _threads;
  std::priority_queue<TaskEntry> _tasks;
};

template <class F, class... A>
decltype(auto) ThreadPool::enqueue(F &&f, A &&... a) {
  return enqueue(0, std::forward<F>(f), std::forward<A>(a)...);
}

template <class F, class... A>
decltype(auto) ThreadPool::enqueue(int priority, F &&f, A &&... a) {
  using _Ret = std::invoke_result_t<F, A...>;

  auto task = Task(std::packaged_task<_Ret()>(
      std::bind(std::forward<F>(f), std::forward<A>(a)...)));

  std::future<_Ret> future = task.get_future();

  {
    std::unique_lock lock(_mutex);

    if (_stopped)
      throw std::runtime_error(
          "Can not enqueue a task on an already closed ThreadPool");

    std::unique_ptr<ITask> ptr =
        std::make_unique<Task<std::packaged_task<_Ret()>>>(std::move(task));

    _tasks.push(TaskEntry(priority, std::move(ptr)));
  }

  // Notify a single thread about the new task.
  _condvar.notify_one();

  return future;
}
