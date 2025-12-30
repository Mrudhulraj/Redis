# Welcome to My Cpp Redis Database (Composed of Client and Server)
***

## Tasks
This project is a **Redis Database - Client-Server Architecture** written in **C++**.  

**Client**
It allows users to **connect to a Redis server** via **TCP sockets**, send **Redis commands**,  
and **receive and parse responses** using the **RESP protocol**.

**Server**
A lightweight Redis-compatible in-memory data store written in C++. Supports strings, lists, and hashes, full Redis Serialization Protocol (RESP) parsing, multi-client concurrency, and periodic disk persistence.

This implementation supports:
- **Interactive REPL Mode** (like the official Redis CLI)
- **One-shot execution Mode** (run a single Redis command from the terminal)
- **Networking** using Berkeley sockets (IPv4 & IPv6 support)
- **RESP Protocol Parsing** for handling Redis responses (RESP2).

---

## Description
The Redis CLI and Server was implemented using **C++** with a focus on:
**Low-level TCP connection to Redis**  
**Supports Redis commands (SET, GET, PING, etc.)**  
**Interactive REPL shell**  
**One-shot execution mode**  
**Properly formatted RESP protocol parsing**  
**Cross-platform compatibility (Linux & macOS)**  
**Object-oriented design with separate classes** 

The Redis Server Operations allowed include:
**Default Port:** `6379`
- **Common Commands:** ECHO, FLUSHALL
- **Key/Value:** SET, GET, KEYS, TYPE, DEL/UNLINK, EXPIRE, RENAME
Data is automatically dumped to `dump.my_rdb` every 300 seconds and on graceful shutdown; at startup, the server attempts to load from this file.


---

## Project Structure
This project follows a **modular OOP architecture**:

```
├── redis-client
│   ├── Makefile
│   ├── bin
│   │   └── my_redis_cli
│   ├── build
│   │   ├── CLI.o
│   │   ├── CommandHandler.o
│   │   ├── RedisClient.o
│   │   ├── ResponseParser.o
│   │   └── main.o
│   └── src
│       ├── CLI.cpp
│       ├── CLI.h
│       ├── CommandHandler.cpp
│       ├── CommandHandler.h
│       ├── RedisClient.cpp
│       ├── RedisClient.h
│       ├── ResponseParser.cpp
│       ├── ResponseParser.h
│       └── main.cpp
├── redis-server
│   ├── Makefile
│   ├── build
│   │   ├── RedisCommandHandler.d
│   │   ├── RedisCommandHandler.o
│   │   ├── RedisDatabase.d
│   │   ├── RedisDatabase.o
│   │   ├── RedisServer.d
│   │   ├── RedisServer.o
│   │   ├── main.d
│   │   └── main.o
│   ├── include
│   │   ├── RedisCommandHandler.h
│   │   ├── RedisDatabase.h
│   │   └── RedisServer.h
│   ├── redis_server
│   └── src
│       ├── RedisCommandHandler.cpp
│       ├── RedisDatabase.cpp
│       ├── RedisServer.cpp
│       └── main.cpp
└── usecases.md
```

| Component        | Responsibility |
|------------------|----------------|
| `RedisClient`    | Handles TCP socket connections and sending commands to Redis. |
| `CommandHandler` | Parses user input and formats Redis commands into RESP format. |
| `ResponseParser` | Parses responses from Redis (Simple Strings, Errors, Bulk Strings, Arrays, etc.). |
| `CLI`            | Manages the **REPL loop**, user input, and command execution. |
| `main.cpp`       | Parses command-line arguments and launches the CLI. |

---

## Installation
You can use terminal to build, make and use.

### **Requirements**
- **G++ (C++ Compiler)**
- **Make**

---

### **Build Instructions for Redis Client**
```sh
git clone https://github.com/Mrudhulraj/Redis.git
cd redis-client
make
```
This will generate the executable **`bin/my_redis_cli`**.

---

## Usage
After  generating the executable **`bin/my_redis_cli`**, you can use it.

### Start Interactive Mode (REPL)
```
./bin/my_redis_cli
```
Example:
```
127.0.0.1:6379> set mykey hello
OK
127.0.0.1:6379> get mykey
"hello"
127.0.0.1:6379> quit
Goodbye.
```
---

## **Build Instructions for Redis Server**
This project uses a simple Makefile. Ensure you have a C++17 (or later) compiler.
- `make`
- `make clean`

```bash
# from project root
make
```
## Usage
After compiling the server, you can run it and use with client.

### Running the Server

Start the server on the default port (6379) or specify a port:

```bash
./my_redis_server            # listens on 6379
./my_redis_server 6380       # listens on 6380
```

On startup, the server attempts to load `dump.my_rdb` if present:
```
Database Loaded From dump.my_rdb
# or
No dump found or load failed; starting with an empty database.
```

A background thread dumps the database every 5 minutes.

To gracefully shutdown and persist immediately, press `Ctrl+C`.

---

### Using the Server

You can connect with the standard `redis-cli` or custom RESP client `my_redis_cli`.

```bash
# Using redis-cli:
redis-cli -p 6379

# Example session:
127.0.0.1:6379> PING
PONG

127.0.0.1:6379> SET foo "bar"
OK

127.0.0.1:6379> GET foo
"bar"
```

---

## Supported Commands

### Common
- **PING**: `PING` → `PONG`
- **ECHO**: `ECHO <msg>` → `<msg>`
- **FLUSHALL**: `FLUSHALL` → clear all data

### Key/Value
- **SET**: `SET <key> <value>` → store string
- **GET**: `GET <key>` → retrieve string or nil
- **KEYS**: `KEYS *` → list all keys
- **TYPE**: `TYPE <key>` → `string`/`list`/`hash`/`none`
- **DEL/UNLINK**: `DEL <key>` → delete key
- **EXPIRE**: `EXPIRE <key> <seconds>` → set TTL
- **RENAME**: `RENAME <old> <new>` → rename key

---

## Design & Architecture

- **Concurrency:** Each client is handled in its own `std::thread`.  
- **Synchronization:** A single `std::mutex db_mutex` guards all in-memory stores.  
- **Data Stores:**  
  - `kv_store` (`unordered_map<string,string>`) for strings  
  - `list_store` (`unordered_map<string,vector<string>>`) for lists  
  - `hash_store` (`unordered_map<string,unordered_map<string,string>>`) for hashes
- **Expiration:** Lazy eviction on each access via `purgeExpired()`, plus TTL map `expiry_map`.  
- **Persistence:** Simplified RDB: text‐based dump/load in `dump.my_rdb`.  
- **Singleton Pattern:** `RedisDatabase::getInstance()` enforces one shared instance.  
- **RESP Parsing:** Custom parser in `RedisCommandHandler` supports both inline and array formats.
