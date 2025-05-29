#include "generate_tests.h"
#include "command_processor.h"

// for generating test data
#include <chrono>
#include <thread>
#include <fstream>

#include "async.h"
#include <iostream>
#include <vector>

void client_task(std::shared_ptr<async::Handle> handle, const std::vector<std::string>& commands) {
    for (const auto& cmd : commands) {
        handle->receive(cmd.c_str(), cmd.size());
    }
}

std::vector<std::string> readCommandsFromFile(const std::string& filename) {
    std::vector<std::string> commands;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {  
            commands.push_back(line);
        }
    }

    return commands;
}

int main() {
    // create 2 processing contexts with bulk_size 3 and 2
    auto handle1 = async::connect(3);
    auto handle2 = async::connect(2);

    const std::string input_file1 = "commands1.txt";
    generateTestFile(input_file1, "cmd_");

    const std::string input_file2 = "commands2.txt";
    generateTestFile(input_file2, "prc_"); //prc_ and cmd_ to visually distinguish threads

    // Data for processing
    std::vector<std::string> data1 = readCommandsFromFile(input_file1);
    std::vector<std::string> data2 = readCommandsFromFile(input_file2);

    //client_task(handle1, data1);
    //client_task(handle2, data2);

    // Processing in 2 threads
    std::thread t1([&]() { client_task(handle1, data1); });
    std::thread t2([&]() { client_task(handle2, data2); });

    t1.join();
    t2.join();

    // Dtors of handle1 and handle2 will automatically call disconnect()

    return 0;
}
