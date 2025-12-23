#include<iostream>
#include "../include/RedisDatabase.h"
#include "../include/RedisServer.h"
#include <thread>
#include <chrono>

int main(int argc,char* argv[]){
    int port = 6379;
    if(argc>=2) port = std::stoi(argv[1]);


    RedisServer server(port);

    // Dump the database every 300 sec
    std::thread persistanceThread([](){
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(300));
            // Dump or sync db
            if(!RedisDatbase::getInstance().dump("dump.my_rdb")){
                std::cerr<<"Error dumping database\n";
            }
            else{
                std::cout<<"Dumped Database to dump.my_rdb\n";
            }
        }
    });

    persistanceThread.detach();

    server.run();
    return 0;
}
