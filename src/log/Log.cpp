#include "Log.hpp"

Log::Log() {
    
}

Log::~Log() {

}

void Log::print(const char* text) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ofstream log_file("log.txt", std::fstream::out | std::fstream::app);
    log_file << std::put_time(&tm, "%d.%m.%Y %H:%M:%S ") << text << std::endl;
    log_file.close();
}

void Log::print(int num) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ofstream log_file("log.txt", std::fstream::out | std::fstream::app);
    log_file << std::put_time(&tm, "%d.%m.%Y %H:%M:%S ") << num << std::endl;
    log_file.close();
}