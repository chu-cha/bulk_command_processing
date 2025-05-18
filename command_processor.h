#pragma once

#include <vector>
#include <string>
#include <iostream>

class CommandProcessor
{
    using BufferType = std::vector<std::string>;

public:
    explicit CommandProcessor(size_t b_size)
        : bulk_size(b_size) {
    }

    void process(const std::string& command);

private:
    void handle_block_start();
    void handle_block_end();
    void add_command(const std::string& cmd);
    bool in_dynamic_block() const;
    void check_flush_conditions();
    bool should_flush() const;
    void flush(bool force = false);
    void print_buffer_impl(std::ostream& output_stream) const;
    void print_buffer() const;
    void clear_state();

private:

    BufferType buffer;
    size_t open_braces = 0;
    bool dynamic_block_just_started = false;
    bool dynamic_block_just_ended = false;
    const size_t bulk_size;

};
