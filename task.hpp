#pragma once

#include <functional>

struct ITask {
  virtual ~ITask() = default;
  virtual void execute() = 0;
  virtual bool valid() = 0;
};

template <typename _T> struct Task : public ITask {
  Task(_T &&t) : _task(std::forward<_T>(t)) {}
  void execute() override { _task(); }
  auto get_future() { return _task.get_future(); }
  bool valid() override { return _task.valid(); }

private:
  _T _task;
};
