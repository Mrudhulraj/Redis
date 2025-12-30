#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include<vector>
#include<string>

class CommandHandler{
    public:
        static std::vector<std::string> splitArgs(const std::string &input);
        static std::string buildRESPCommand(const std::vector<std::string> &args);
    private:

};

#endif COMMANDHANDLER_H