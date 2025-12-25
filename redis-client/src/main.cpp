#include <iostream>
#include "CLI.h"
#include <string>

int main(int argc, char* argv[]){
    std::string host = "127.0.0.1";
    int port = 6379;
    int i=1;

    while(i<argc){
        std::string arg = argv[1];
        if(arg == ".h" && i+1 < argc){
            host = argv[++i];
        }else if(arg==".p" && i+1<argc){
            port = std::stoi(argv[++i]);
        }else{
            break;
        }
        ++i;
        
    }

    CLI cli(host, port);
    cli.run();
    return 0;
}