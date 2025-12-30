#include "CLI.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 6379;
    int i = 1;
    std::vector<std::string> commandArgs;

    // Parse command-line args for -h and -p
    while (i < argc) {
        std::string arg = argv[i];
        if (arg == "-h" && i + 1 < argc) {// -h 127.0.0.1
            host = argv[++i];
        } else if (arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else {
            // Remaining args
            while (i <argc) {
                commandArgs.push_back(argv[i]);
                i++;
            }
            break;
        }
        ++i;
    }

    // Handle REPL and one-shot command modes
    CLI cli(host, port);
    cli.run(commandArgs);

    return 0;
}