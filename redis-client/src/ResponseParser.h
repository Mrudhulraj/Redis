#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H

#include <string>

class ResponseParser{
    public:
        static std::string parseResponse(int sockfd);

    private:
        static std::string parseSimpleString(int sockfd);
        static std::string parseSimpleError(int sockfd);
        static std::string parseInteger(int sockfd);
        static std::string parseBulkString(int sockfd);
        static std::string parseArray(int sockfd);
};

#endif