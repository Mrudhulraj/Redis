#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include <string>

class RedisDatbase{
    public:
        static RedisDatbase &getInstance();

        bool dump(const std::string& filename);
        bool load(const std::string& filename);
        
    private:
        RedisDatbase() = default;
        ~RedisDatbase() = default;
        RedisDatbase(const RedisDatbase&) = delete;
        RedisDatbase& operator=(const RedisDatbase&)=delete;
};

#endif