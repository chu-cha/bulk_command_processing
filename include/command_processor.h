#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>

#include "thread_safe_queue.h"

class CommandProcessor
{
    using BufferType = std::vector<std::string>;

public:

    explicit CommandProcessor(size_t b_size);
    ~CommandProcessor();

    void process(const std::string& command);

private:

    void handle_block_start();
    void handle_block_end();
    void add_command(const std::string& cmd);
    bool in_dynamic_block() const;
    void check_flush_conditions();
    bool should_flush() const;
    void flush(bool force = false);
    void print_buffer_impl(std::ostream& output_stream, const  std::vector<std::string>& commands) const;
    void clear_state();

    void file_worker(int worker_id);
    void log_worker();
    std::string generate_filename(int worker_id) const;

private:

    BufferType buffer;
    size_t open_braces = 0;
    bool dynamic_block_just_started = false;
    bool dynamic_block_just_ended = false;
    const size_t bulk_size;

    // Thread-safe queues for workers
    ThreadSafeQueue log_queue_;
    ThreadSafeQueue file_queue_;

    // Worker threads
    std::thread log_thread_;
    std::thread file_thread1_;
    std::thread file_thread2_;

};
