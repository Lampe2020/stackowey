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

bool dev_mode= false;
const std::vector<uint64_t> stack{ 42ull };
std::vector<std::string> playfield;

bool read_source_from_stream(std::istream& source) {
    if (source.fail())
        return false;
    std::string line;
    std::getline(source, line);
    if (line[0] == '#' && line[1] == '!')
        std::getline(source, line); // Ignore the shebang line if it exists
    playfield.push_back(line);
    const uint64_t linesize= playfield[0].size();
    while (std::getline(source, line)) {
        if (line.size() != linesize)
            error_out(E_SYNTAX, "Source file malformatted: not rectangular!");
        playfield.push_back(line);
    }
    return true;
}

bool read_source(const char* source_path) {
    std::ifstream source(source_path);
    return read_source_from_stream(source);
}

int main(int argc, char* argv[], char* envp[]) {
    std::atexit(cleanup);

    for (int i= 0; i < argc; i++)
        std::cout << argv[i] << std::endl;

    if (argc == 1) {
        read_source_from_stream(std::cin);
    } else if (argc >= 2 && argv[1][0] != '-') {
        read_source(argv[1]);
    } else if (argc > 1 && argv[1][0] == '-') {
        if (argv[1] == "-d" || argv[1] == "--dev") {
            std::cout << "Development mode enabled!" << std::endl;
            dev_mode= true;
        }
        if (argc >= 2)
            read_source(argv[2]);
        else
            read_source_from_stream(std::cin);
    } else
        read_source(argv[1]);
    
    //TODO: Start parsing!

    return 0;
}
