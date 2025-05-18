#include "command_processor.h"

#include <fstream>
#include <chrono>
#include <sstream>


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

    if (!force)
        print_buffer();
    clear_state();
}

void CommandProcessor::print_buffer_impl(std::ostream& output_stream) const
{
    auto sz = buffer.size();

    output_stream << "bulk: ";
    for (auto i = 0; i < sz; i++) {
        output_stream << buffer[i] << (i < sz - 1 ? ", " : "");
    }
    output_stream << '\n';
}

void CommandProcessor::print_buffer() const {

    auto sz = buffer.size();

    if (!sz)
        return;

    print_buffer_impl(std::cout);

    auto generate_filename = []() {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << "bulk" << timestamp << ".log";
        return oss.str();
        };

    std::ofstream logfile(generate_filename());
    print_buffer_impl(logfile);
}

void CommandProcessor::clear_state() {
    buffer.clear();
    dynamic_block_just_started = false;
    dynamic_block_just_ended = false;
}


