#include "CLI.h"

CLI::CLI(const std::string& host, int port) : redisClient(host,port){}

void CLI::run(){
    if(!redisClient.connectToServer()){
        return;
    }
    
    std::cout << "Connected to redsis server at \n";
}