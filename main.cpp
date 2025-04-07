// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
#include "bitset.hpp"
#include "magic_enum.hpp"
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void cleanup() {
    // If there is some manual cleanup to do, do it in here!
}

enum ErrorCode { E_SUCCESS= 0, E_EXIT= 1, E_SYNTAX= 2 };

std::string getErrorName(ErrorCode err) {
    auto name= magic_enum::enum_name(err);
    return name.empty() ? "E_UNKNOWN" : std::string(name);
}

void error_out(ErrorCode err, std::string msg) {
    if (err == E_SUCCESS) {
        std::cerr << getErrorName(err) << '[' << err << "]: "
                  << (((bool)(time(NULL) % 2)) ?
                      "Mission accomplished. That’s the problem" :
                      "Unexpected lack of failure")
                  << '.' << std::endl;
                  return; // Try to continue
    }
    else if (err == E_EXIT)
        std::exit(0);
    else
        std::cerr << "Uncaught " << getErrorName(err) << '[' << err << ']' << ": "
                  << ((bool)msg.size() ? msg : std::to_string(err)) << std::endl;
    std::exit(err);
}

const std::vector<uint64_t> stack{ 42ull };

char** source;

bool read_source(const char* source_path) {

    std::ifstream source(source_path);
    if (source.fail())
        return false;
    std::string line;
    std::getline(source, line);
    if (line[0] == '#' && line[1] == '!')
        std::getline(source, line); // Ignore the shebang line if it exists
    const uint linelen= line.size();
    while (!source.eof()) {
        if (line.size() != linelen)
            error_out(E_SYNTAX, "Source file malformatted: not rectangular!");
        std::getline(source, line);
    }
    return true;
}

int main(int argc, char* argv[], char* envp[]) {
    std::atexit(cleanup);
    for (int i= 0; i < argc; i++)
        std::cout << argv[i] << std::endl;

    //TODO: parse the arguments and interpret the specified source file

    return 0;
}
