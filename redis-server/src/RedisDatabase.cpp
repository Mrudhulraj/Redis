#include "../include/RedisDatabase.h"
#include <fstream>
#include <sstream>
#include <unordered_map>


RedisDatabase& RedisDatabase::getInstance(){
    static RedisDatabase instance;
    return instance;
}

bool RedisDatabase::flushAll(){
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store.clear();
    list_store.clear();
    hash_store.clear();
    return true;
}

void RedisDatabase::set(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store[key] = value;
}

std::vector<std::string> RedisDatabase::keys(){
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<std::string> Result;
    for(auto pair: kv_store){
        Result.push_back(pair.first);
    }
    return Result;
}

bool RedisDatabase::get(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(key);
    if(it != kv_store.end())
        return true;
}

bool RedisDatabase::del(const std::string& key){
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(key);
    if(it!= kv_store.end()){
        kv_store.erase(it);
        return true;
    }else return false;
}

std::string RedisDatabase::type(const std::string& key){
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(key);
    if(it!= kv_store.end()){
        return "string";
    }
    return "none";
}

bool RedisDatabase::expire(const std::string& key, int seconds){
    return false;
}

bool RedisDatabase::rename(const std::string& oldkey, const std::string& newKey){
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(oldkey);
    if(it != kv_store.end()){
        kv_store[newKey]=it->second ;  
        kv_store.erase(it);
        return true;
    }else
    return false;
}




bool RedisDatabase::dump(const std::string& filename){
    
    std::lock_guard<std::mutex> lock(db_mutex);
    std::ofstream ofs(filename,std::ios::binary);
    if(!ofs) return false;

    for (const auto& kv: kv_store){
        ofs << "K" << kv.first << " " << kv.second << "\n";
    }
    for(const auto& kv: list_store){
        ofs << "L " << kv.first;
        for (const auto& item: kv.second){
            ofs << " " << item;
        }
        ofs << "\n";
    }
    for (const auto& kv: hash_store){
        ofs << "H " << kv.first;
        for(const auto& value: kv.second){
            ofs << " " << value.first << " " <<value.second;
        }
        ofs << "\n";
    }

    return true;
}

bool RedisDatabase::load(const std::string& filename){
    std::lock_guard<std::mutex> lock(db_mutex);
    std::ifstream ifs(filename,std::ios::binary);
    if(!ifs) return false;

    kv_store.clear();
    list_store.clear();
    hash_store.clear();

    std::string line;
    while(std::getline(ifs,line)){
        std::istringstream iss(line);
        char type;
        iss >> type;
        if(type=='K'){
            std::string key,value;
            iss >> key >> value;
            kv_store[key] =value;
        }else if(type == 'L'){
            std::string key;
            iss >> key;
            std::string item;
            std::vector<std::string> list;
            while(iss>>item){
                list.push_back(item);
            }
            list_store[key]=list;
        }else if(type == 'H'){
            std::string key;
            iss >> key;
            std::unordered_map<std::string,std::string> hash;
            std::string pair;
            while(iss>>pair){
                auto pos = pair.find(':');
                if( pos != std::string::npos){
                    std::string field =pair.substr(0,pos);
                    std::string value = pair.substr(pos+1);
                    hash[field]=value;
                }
            }
        }
    }
    return true;
}