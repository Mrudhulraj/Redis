#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include<vector>
#include<string>

class CommandHandler{
    public:
        static std::vector<std::string> splitArgs(const std::string &input);
        static std::string buildRESPcommand(const std::vector<std::string> &args);
    private:

};

#endif