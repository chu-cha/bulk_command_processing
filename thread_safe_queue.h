#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue {
public:
    void push(const std::vector<std::string>& commands);
    bool pop(std::vector<std::string>& commands);
    void stop();

private:
    std::queue<std::vector<std::string>> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool stop_ = false;
};
