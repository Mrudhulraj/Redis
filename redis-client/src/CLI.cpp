#include "CLI.h"

static std::string trim(const std::string &s){
    size_t start = s.find_first_not_of("\t\n\r\f\v");
    if(start == std::string::npos) return "";
    size_t end = s.find_last_not_of("\t\n\r\f\v");
    return s.substr(start, end-start +1);
}

CLI::CLI(const std::string& host, int port) : redisClient(host,port){}

void CLI::run(){
    if(!redisClient.connectToServer()){
        return;
    }
    
    std::cout << "Connected to redsis server at"<< redisClient.getSocketFD() <<"\n";
    std::string  host = "1270.0.0.1";
    int port  = 6379;

    while(true){
        std::cout << host << ":" << port << ">" ;
        std::cout.flush();
        std::string line;
        if(!std::getline(std::cin,line)) break;
        line = trim(line);
        if(line.empty()) continue;
        if(line == "quit") {
            std::cout << "Goodbye \n";
            break;
        }

        if(line == "help"){
            std::cout << "Displaying help \n";
            continue;
        }

        // Split command to tokens
        std::vector<std::string> args = CommandHandler::splitArgs(line);
        if(args.empty()) continue;
        std::string command = CommandHandler::buildRESPCommand(args);

        if(redisClient.sendCommand(command)){
            std::cerr << "Failed to execute command \n";
            break;
        }
    }
}