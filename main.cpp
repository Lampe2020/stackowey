// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
// #include "bitset.hpp" // Commented out until it's needed, to silence code check warning
#include "magic_enum.hpp"
#include "utf8.cpp"
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

enum Direction {
    RIGHT= uint8_t(0),
    DOWN = uint8_t(1),
    LEFT = uint8_t(2),
    UP   = uint8_t(3),
};

bool source_from_stdin= false;
bool dev_mode         = false;
std::ifstream source_filestream;
std::ofstream debug_info;
std::vector<uint64_t> stack{ 42ull };
std::vector<const char*> playfield;
uint64_t linelen;
uint64_t pos[2]= { 0ull, 0ull };
std::vector<std::string> input;
Direction direction= RIGHT;

void cleanup() {
    std::cerr.flush();
    std::cout.flush();
    if (dev_mode) {
        debug_info << "\n\n--- end of log ---" << std::endl;
        debug_info.flush();
        debug_info.close();
    }
}

enum ErrorCode {
    E_SUCCESS = uint8_t(0),
    E_EXIT    = uint8_t(1),
    E_SYNTAX  = uint8_t(2),
    E_3D      = uint8_t(3),
    E_STREAM_R= uint8_t(4),
    E_STREAM_W= uint8_t(5)
};

std::string getErrorName(ErrorCode err) {
    auto name= magic_enum::enum_name(err);
    return name.empty() ? "E_UNKNOWN" : std::string(name);
}

void error_out(ErrorCode err, std::string msg) {
    if (err == E_SUCCESS) {
        std::cerr << getErrorName(err) << '[' << err
                  << "]: Unexpected lack of failure." << std::endl;
        return; // Try to continue
    } else if (err == E_EXIT)
        std::exit(0);
    else
        std::cerr << getErrorName(err) << '[' << err << ']' << ": "
                  << ((bool)msg.size() ? msg : std::to_string(err)) << std::endl;
    std::exit(err);
}

void read_source_from_stream(std::istream& source) {
    source_from_stdin= (&source == &std::cin);
    if (source.fail())
        error_out(E_STREAM_R, "Could not read source stream!");
    std::string line;
    std::getline(source, line);
    if (line[0] == '#' && line[1] == '!')
        std::getline(source, line); // Ignore the shebang line if it exists
    playfield.push_back(line.c_str());
    linelen= line.size();
    debug_info << "Detected width " << linelen << std::endl;
    while (std::getline(source, line) &&
           !(source_from_stdin && line == "---")) {
        if (line.size() != linelen)
            error_out(E_SYNTAX, "The eastward edge is too rough!");
        playfield.push_back(line.c_str());
    }
}

void close_source() {
    source_filestream.close();
}

void read_source(const char* source_path) {
    std::ifstream source(source_path);
    std::atexit(close_source);
    read_source_from_stream(source);
}

void get_input() {
    if (dev_mode)
        debug_info << "Waiting for user input." << std::endl;
    std::string line;
    std::getline(std::cin, line);
    if (dev_mode)
        debug_info << "Got input from user:\n" << line << std::endl;
    uint64_t next_stack_value;
    std::vector<char32_t> unicode_line= unicodepoints(line);
    for (uint64_t i= 0; i < line.size(); i++) {
        stack.push_back(uint64_t(unicode_line[i]));
    }
    stack.push_back(0ull);
}

void send_output() {
    if (dev_mode)
        debug_info << "Outputting U+" << stack.back() << std::endl;
    std::cout << utf8_encode(stack.back() % UINT32_MAX); // Modulo to prevent invalid codepoint
    stack.pop_back(); // Actually remove the value from the stack
}

void log_instruction() {
    
    if (dev_mode)
        debug_info << 0;
}

int main(int argc, char* argv[], char* envp[]) {
    std::locale::global(std::locale("C.UTF-8"));

    std::atexit(cleanup);

    if (argc == 1) {
        std::cout << "End the program with a line of only three dashes "
                     "(\"---\", with no spaces or other characters before or "
                     "after) to separate it from its input"
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
        if (argc >= 3)
            read_source(argv[2]); // FIXME: Why does this crash?
        else
            read_source_from_stream(std::cin);
    } else
        read_source(argv[1]);

    while (true) {
        if (dev_mode)
            debug_info << "";
        switch (playfield[pos[0] % playfield.size()][pos[1] % linelen]) {
            default: {
                // Ignore unrecognized character
            }
        }
        direction= Direction(direction & 0b11); // Ensure the direction cannotbe invalid
        switch (direction) {
            case RIGHT: {
                pos[1]++;
                break;
            }
            case DOWN: {
                pos[0]++;
                break;
            }
            case LEFT: {
                pos[1]--;
                break;
            }
            case UP: {
                pos[0]--;
                break;
            }
            default: {
                error_out(E_3D, "I'm flattered!");
            }
        }
    }

    return 0;
}
