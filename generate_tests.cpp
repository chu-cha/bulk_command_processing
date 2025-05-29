#include "generate_tests.h"

#include <random>
#include <fstream>

void generateTestFile(const std::string& filename, std::string cmd_prefix) {
    std::ofstream out(filename);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> special_dist(0, 15);

    int open_braces = 0;
    int command_count = 0;

    while (command_count < 100 || open_braces > 0) {
        int action = special_dist(gen);
        std::string next_token;

        if (action < 14) {
            next_token = cmd_prefix + std::to_string(command_count);
            command_count++;
        }
        else if (action == 14) {
            next_token = "{";
            open_braces++;
        }
        else if (action == 15 && open_braces > 0) {
            next_token = "}";
            open_braces--;
        }
        else {
            continue; // dismiss non-valid tokens
        }

        if (!next_token.empty()) {
            out << next_token << "\n"; // Add \n only after non-empy tokens
        }
    }

}