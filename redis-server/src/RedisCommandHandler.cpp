#include "../include/RedisCommandHandler.h"
#include "../include/RedisDatabase.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

// RESP Parser 

std::vector<std::string> parseRespCommand(const std::string &input){
    std::vector<std::string> tokens;
    if(input.empty()) return tokens;

    if(input[0]!='*'){
        std::istringstream iss(input);
        std::string token;
        while(iss>>token){
            tokens.push_back(token);
        }
        return tokens;
    }

    size_t pos = 0;
    // Expect '*' followed by number of elements
    if(input[pos] != '*') return tokens;
    pos++;

    size_t crlf = input.find("\r\n",pos);
    if(crlf == std::string::npos) return tokens;

    int  numElements= std::stoi(input.substr(pos,crlf-pos));
    pos=crlf+2;

    for(int i = 0;i<numElements;i++){
        if(pos >= input.size() || input[pos] != '$') break; // Format error
        pos++; //skip '$'
        
        crlf = input.find("\r\n",pos);
        if(crlf == std::string::npos) break;
        int len = std::stoi(input.substr(pos,crlf-pos));
        pos=crlf+2;

        if(pos + len > input.size()) break;
        std::string token = input.substr(pos,len);
        tokens.push_back(token);
        pos+=len+2;

    } 
    return tokens;
}

RedisCommandHandler::RedisCommandHandler(){ }

std::string RedisCommandHandler::processCommand(const std::string&commandLine){
    auto tokens = parseRespCommand(commandLine);
    if(tokens.empty()){
        return "-Error: Empty command\r\n";
    }
    for(auto &t:tokens){
        std::cout<< t << "\n";
    }
    std::string cmd = tokens[0];
    std::transform(cmd.begin(),cmd.end(),cmd.begin(),::toupper);

    std::ostringstream response;
    // Connect to DB
    RedisDatabase & db = RedisDatabase::getInstance();
    
    //Check commands
    if(cmd=="PING"){
        response << "+PONG\r\n";
    }else if(cmd== "ECHO"){
        if(tokens.size() <2){
            response << "-Eror: ECHO requires a message\r\n";
        }else{
            response << "+" << tokens[1] << "\r\n";
        }
    }else if (cmd == "FLUSH"){
        db.flushAll();
        response << "+OK\r\n";
    }else if(cmd == "SET"){
        if(tokens.size()<3)
            response << "-Error requires key value\r\n";
        else{
            db.set(tokens[1],tokens[2]);
            response<< "+OK\r\n";
        }
    }else if (cmd == "SET"){
         if(tokens.size()<2)
            response << "-Error requires keyr\n";
        else{
            std::string value;
            if(db.get(tokens[1],value))
                response<< "$"<< value.size() << "\r\n" << value << "\r\n";
            else 
                response << "$-1\r\n";
        }
    } else if(cmd == "KEYS"){
        std::vector<std::string> allKeys = db.keys();
        response << "*" << allKeys.size() << "\r\n";
        for(const auto& key:allKeys){
            response << "$" << key.size() << "\r\n" << key << "\r\n";
        }
    }else if(cmd=="TYPE"){
        if(tokens.size()<2)
            response << "-Error: TYPE requires key \r\n";
        else 
            response << "+" << db.type(tokens[1]) << "\r\n"; 
    }else if(cmd=="DEL" || cmd == "UNLINK"){
        if(tokens.size() < 2)
            response << "-Error: DEL requires key\r\n";
        else{
            bool res = db.del(tokens[1]);
            response << ":" << (res ? 1:0) << "\r\n"; 
        }
    }else if(cmd == "EXPIRE"){
        if(tokens.size()<3)
            response << "-Error requires key and time\r\n";
        else
            if(db.expire(tokens[1],std::stoi(tokens[2])))
                response << "+OK \r\n";
    }else if(cmd == "RENAME"){
        if(tokens.size()<3)
            response << "-Error requires key and time\r\n";
        else
            if(db.rename(tokens[1],tokens[2]))
                response << "+OK \r\n";
    }
    else{
        response << "Error: Unknow command\r\n";
    }

    return response.str();
}