#include "command_processor.h"

#include <fstream>
#include <chrono>
#include <sstream>


CommandProcessor::CommandProcessor(size_t b_size)
    : bulk_size(b_size) {
    // Start worker threads
    log_thread_ = std::thread(&CommandProcessor::log_worker, this);
    file_thread1_ = std::thread(&CommandProcessor::file_worker, this, 1);
    file_thread2_ = std::thread(&CommandProcessor::file_worker, this, 2);
}
CommandProcessor::~CommandProcessor() {
    // Signal threads to stop
    log_queue_.stop();
    file_queue_.stop();

    // Wait for threads to finish
    if (log_thread_.joinable()) log_thread_.join();
    if (file_thread1_.joinable()) file_thread1_.join();
    if (file_thread2_.joinable()) file_thread2_.join();
}

void CommandProcessor::process(const std::string& command) {
    if (command == "{") {
        handle_block_start();
    }
    else if (command == "}") {
        handle_block_end();
    }
    else if (command == "EOF") {
        flush(true);
    }
    else {
        add_command(command);
    }

    check_flush_conditions();
}

void CommandProcessor::handle_block_start() {
    if (!in_dynamic_block()) {
        flush();
        dynamic_block_just_started = true;
    }
    open_braces++;
}

void CommandProcessor::handle_block_end() {
    if (open_braces > 0) {
        open_braces--;

        if (!in_dynamic_block()) {
            dynamic_block_just_ended = true;
        }
    }
}

void CommandProcessor::add_command(const std::string& cmd) {
    buffer.push_back(cmd);
}

bool CommandProcessor::in_dynamic_block() const {
    return open_braces > 0;
}

void CommandProcessor::check_flush_conditions() {
    if (should_flush()) {
        flush();
    }
}

bool CommandProcessor::should_flush() const {

    if (dynamic_block_just_started) return true;

    if (!in_dynamic_block()) {
        return dynamic_block_just_ended || buffer.size() == bulk_size;
    }
    return false;
}

void CommandProcessor::flush(bool force) {

    if (!force && !buffer.empty()) {
        // Send to log thread
        log_queue_.push(buffer);

        // Send to file threads
        file_queue_.push(buffer);
    }

    clear_state();
}

void CommandProcessor::log_worker() {
    std::vector<std::string> commands;
    while (log_queue_.pop(commands)) {
        print_buffer_impl(std::cout, commands);
    }
}

void CommandProcessor::file_worker(int worker_id) {
    std::vector<std::string> commands;
    while (file_queue_.pop(commands)) {
        auto filename = generate_filename(worker_id);
        std::ofstream logfile(filename);
        print_buffer_impl(logfile, commands);
    }
}

std::string CommandProcessor::generate_filename(int worker_id) const {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // Get milliseconds for more precise timing
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << "bulk" << timestamp << "_" << ms.count() << "_" << worker_id << ".log";
    return oss.str();
}

void CommandProcessor::print_buffer_impl(std::ostream& output_stream, const  std::vector<std::string>& commands) const
{
    auto sz = commands.size();

    output_stream << "bulk: ";
    for (auto i = 0; i < sz; i++) {
        output_stream << commands[i] << (i < sz - 1 ? ", " : "");
    }
    output_stream << '\n';
}


void CommandProcessor::clear_state() {
    buffer.clear();
    dynamic_block_just_started = false;
    dynamic_block_just_ended = false;
}


