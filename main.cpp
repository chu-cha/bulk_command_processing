#include "generate_tests.h"
#include "command_processor.h"

// for generating test data
#include <chrono>
#include <thread>
#include <fstream>

void processCommands(CommandProcessor& processor) {
    std::string command;
    while (std::getline(std::cin, command)) {

        processor.process(command); 

        // for testing purposes: emulates output 1 cmd per second
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {

    //// for testing purposes:
    //// 1. Generate txt file with commands
    const std::string input_file = "commands.txt";
    //generateTestFile(input_file);

    // for testing purposes :
    // 2. stream input from file
    std::ifstream in(input_file);
    std::cin.rdbuf(in.rdbuf());

    // 3. process comands (with delay)
    auto block_size = std::stoi(argv[1]);

    CommandProcessor processor(block_size);
    processCommands(processor);

    return 0;
}