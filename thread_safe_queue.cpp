#include "thread_safe_queue.h"

void ThreadSafeQueue::push(const std::vector<std::string>& commands) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(commands);
    cond_var_.notify_one();
}

bool ThreadSafeQueue::pop(std::vector<std::string>& commands) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_var_.wait(lock, [this] { return !queue_.empty() || stop_; });

    if (stop_ && queue_.empty()) return false;

    commands = std::move(queue_.front());
    queue_.pop();
    return true;
}

void ThreadSafeQueue::stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_ = true;
    cond_var_.notify_all();
}