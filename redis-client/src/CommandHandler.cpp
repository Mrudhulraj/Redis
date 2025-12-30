#include "CommandHandler.h"
#include <regex>


std::vector<std::string> CommandHandler::splitArgs(const std::string &input){
    std::vector<std::string> tokens;

    //Arguments split with spaces
    std::regex rgx(R"((\"[^\"]+\"|\S+))"); 
    auto words_begin = std::sregex_iterator(input.begin(),input.end(),rgx);
    auto words_end = std::sregex_iterator();


    for(auto it = words_begin; it != words_end; ++it){
        std::string token = it->str();
        if(token.size() >= 2 && token.front() == '\"'&& token.back() == '\"'){
            token = token.substr(1,token.size() - 2);
        }
        tokens.push_back(token);
    }
    return tokens;
}

std::string CommandHandler::buildRESPcommand(const std::vector<std::string>&args){
    std::ostringstream oss;
    oss << "*" <<args.size() << "\r\n";

    for(const auto &arg:args){
        oss << "$" << args.size() << "\r\n";
    }
    return oss.str();
}