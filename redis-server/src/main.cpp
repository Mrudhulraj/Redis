#include<iostream>
#include "../include/RedisServer.h"

int main(int argc,char* argv){
    int port = 6379;
    if(argc>=2) port = std::stoi(argv[1]);


    RedisServer server(port);
    return 0;
}
