// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
// #include "bitset.hpp" // Commented out until it's needed, to silence code check warning
#include "magic_enum.hpp"
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

bool source_from_stdin= false;
bool dev_mode         = false;
std::ofstream debug_info;
std::vector<uint64_t> stack{ 42 };
std::vector<std::string> playfield;
uint64_t pos[2]= { 0ull, 0ull };
std::vector<std::string> input;

void cleanup() {
    std::cerr.flush();
    std::cout.flush();
    if (dev_mode) {
        debug_info << "\n\n--- end of log ---" << std::endl;
        debug_info.flush();
        debug_info.close();
    }
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
    } else if (err == E_EXIT)
        std::exit(0);
    else
        std::cerr << "Uncaught " << getErrorName(err) << '[' << err << ']' << ": "
                  << ((bool)msg.size() ? msg : std::to_string(err)) << std::endl;
    std::exit(err);
}

bool read_source_from_stream(std::istream& source) {
    source_from_stdin= (&source == &std::cin);
    if (source.fail())
        return false;
    std::string line;
    std::getline(source, line);
    if (line[0] == '#' && line[1] == '!')
        std::getline(source, line); // Ignore the shebang line if it exists
    playfield.push_back(line);
    const uint64_t linesize= playfield[0].size();
    while (std::getline(source, line) &&
           !(source_from_stdin && line == "---")) {
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

uint64_t as_unicodepoint(char c) {
    // TODO: Implement converting the incoming unicode characters to
    return 0ull; // tmp
}

void get_input() {
    std::string line;
    std::getline(std::cin, line);
    uint64_t next_stack_value;
    for (int i= 0; i < line.size(); i++) {}
    next_stack_value= 0;
    stack.push_back(next_stack_value); // FIXME
}

int main(int argc, char* argv[], char* envp[]) {
    std::locale::global(std::locale("C.UTF-8"));

    std::atexit(cleanup);

    if (argc == 1) {
        std::cout << "End the program with a line of only three dashes "
                     "(\"---\", with no other characters before or after) to "
                     "separate it from its input"
                  << std::endl;
        read_source_from_stream(std::cin);
    } else if (argc >= 2 && argv[1][0] != '-') {
        read_source(argv[1]);
    } else if (argc > 1 && argv[1][0] == '-') {
        if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--dev") == 0) {
            std::cout << "Development mode enabled!" << std::endl;
            dev_mode= true;
            debug_info.open("./swy.log");
            debug_info << "--- start of log ---" << std::endl;
        }
        if (argc >= 2)
            read_source(argv[2]);
        else
            read_source_from_stream(std::cin);
    } else
        read_source(argv[1]);

    while (true) {
        break; // tmp
    }

    return 0;
}
