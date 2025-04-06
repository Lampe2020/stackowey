// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
#include "bitset.hpp"
#include "magic_enum.hpp"
#include <cstdint>
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
    if (err != E_EXIT)
        std::cerr << "Uncaught " << getErrorName(err) << ": "
                  << ((bool)msg.size() ? msg : std::to_string(err)) << std::endl;
    std::exit(err);
}

const std::vector<uint64_t> stack{ 42ull };

char** source;

bool read_source(const char* source_path) {
    std::atexit(cleanup);

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

// Puts the result of a*b into res, if it fits in one uint64_t res[0]=0 and res[1]=a*b, otherwise res[0]=(a*b)%UINT64_MAX and res[1]=(a*b)-((a*b)%UINT64_MAX)
// Thanks to https://www.reddit.com/r/C_Programming/comments/14hfy57/comment/jpaxwr8/
static inline void mul(uint64_t res[2], uint64_t a, uint64_t b) {
    __uint128_t r= (__uint128_t)a * b;
    res[0]       = r >> 64;
    res[1]       = r & (uint64_t)-1;
}

int main(int argc, char* argv[], char* envp[]) {
    for (int i= 0; i < argc; i++)
        std::cout << argv[i] << std::endl;

    //TODO: parse the arguments and interpret the specified source file

    return 0;
}
