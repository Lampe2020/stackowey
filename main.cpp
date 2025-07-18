// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
#include "magic_enum.hpp"
#include "utf8.cpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

enum Direction {
    RIGHT= uint8_t(0),
    DOWN = uint8_t(1),
    LEFT = uint8_t(2),
    UP   = uint8_t(3),
};

const char* direction_names[4]= { "east", "north", "west", "south" }; // FIXME: Why are "north" and "south" swapped compared to Direction?
const std::map<const char, const char*> command_names= {
    { '/', "bounce" },   { '\\', "backbounce" }, { '?', "huh" },
    { '!', "hah" },      { '%', "splot" },       { '0', "oh" },
    { '1', "itch" },     { '2', "nigh" },        { '3', "chan" },
    { '4', "fire" },     { '5', "lamp" },        { '6', "glorp" },
    { '7', "wa-ding" },  { '+', "cross" },       { '_', "stumble" },
    { '#', "grille" },   { '@', "whirlpool" },   { '.', "prick" },
    { '=', "sandwich" }, { '8', "dubring" },     { '9', "tailring" }
};

std::string get_command_name(char command) {
    const std::map<const char, const char*>::const_iterator it=
    command_names.find(command);
    if (it == command_names.end())
        return "impostor";
    else
        return it->second;
}

bool source_from_stdin= false;
bool dev_mode         = false;
std::ifstream source_filestream;
std::ofstream debug_info;
std::vector<uint64_t> stack{ 042ull }; // The stack starts out with a single octal 42
std::vector<std::string> playfield;
uint64_t linelen;
uint64_t pos[2]= { 0ull, 0ull };
std::vector<std::string> input;
Direction direction= RIGHT;

void cleanup() {
    std::cerr.flush();
    std::cout.flush();
    if (dev_mode) {
        debug_info << "Final stack state:\n[\n" << std::oct;
        for (uint64_t stack_element : stack)
            debug_info << "    0o" << stack_element << '\n';
        debug_info << "]\n\n--- end of log ---" << std::dec << std::endl;
        debug_info.flush();
        debug_info.close();
    }
}

enum ErrorCode {
    E_SUCCESS = uint8_t(0),
    E_SYNTAX  = uint8_t(1),
    E_3D      = uint8_t(2),
    E_STREAM_R= uint8_t(3),
    E_STREAM_W= uint8_t(4)
};

std::string getErrorName(ErrorCode err) {
    auto name= magic_enum::enum_name(err);
    return name.empty() ? "E_UNKNOWN" : std::string(name);
}

void error_out(ErrorCode err, std::string msg) {
    if (err == E_SUCCESS) {
        std::cerr << getErrorName(err) << '[' << std::dec << err
                  << "]: Unexpected lack of failure!" << std::endl;
        if (dev_mode)
            debug_info << getErrorName(err) << '[' << std::dec << err
                       << "]: Unexpected lack of failure!" << std::endl;
        return; // Try to continue
    } else {
        std::cerr << getErrorName(err) << '[' << std::dec << err << ']' << ": "
                  << ((bool)msg.size() ? msg : std::to_string(err)) << std::endl;
        if (dev_mode)
            debug_info << getErrorName(err) << '[' << std::dec << err << ']'
                       << ": " << ((bool)msg.size() ? msg : std::to_string(err))
                       << "\nExiting because of error!" << std::endl;
        std::exit(err);
    }
}

std::string n_th(uint64_t n) {
    std::stringstream num_s;
    num_s << std::oct << n;
    std::string num= num_s.str();
    switch (num[num.size() - 1]) {
        case '1': {
            num_s << "st";
            break;
        }
        case '2': {
            num_s << "nd";
            break;
        }
        case '3': {
            num_s << "rd";
            break;
        }
        default: num_s << "th";
    }
    return num_s.str();
}

std::string n_times_char(uint64_t n, char c) {
    std::vector<char> sequence;
    if (n < 1)
        return std::string(""); // Cannot put less than none into a string, so just return an empty string.
    for (uint64_t i= 0; i < n; i++) {
        sequence.push_back(c);
    }
    return sequence.data();
}

uint64_t random_number() {
    static std::mt19937_64 random_number_gen(std::random_device{}());
    return random_number_gen();
}

uint64_t pop_stack() {
    uint64_t value;
    if (stack.size()) {
        value= stack.back();
        stack.pop_back();
        if (dev_mode)
            debug_info << "Popped value 0o" << std::oct << value << " off stack"
                       << std::endl;
    } else {
        value= random_number();
        if (dev_mode)
            debug_info << "Stack was empty!\nGenerated random value 0o" << std::oct
                       << value << " instead" << std::dec << std::endl;
    }
    if (dev_mode)
        debug_info << "Stack size is now 0o" << stack.size() << std::dec << std::endl;
    return value;
}

void push_stack(uint64_t value) {
    stack.push_back(value);
    if (dev_mode)
        debug_info << "Pushed value 0o" << std::oct << value << " to stack\nStack size is now 0o"
                   << std::oct << stack.size() << std::dec << std::endl;
}

void read_source_from_stream(std::istream& source) {
    source_from_stdin= (&source == &std::cin);
    if (source_from_stdin && dev_mode)
        debug_info << "Reading source from stdin..." << std::endl;
    if (source.fail())
        error_out(E_STREAM_R, "Could not read source stream!");
    std::string line;
    std::getline(source, line);
    if (dev_mode)
        debug_info << "Read first line:\n    " << line << std::endl;
    if (line[0] == '#' && line[1] == '!') {
        if (dev_mode)
            debug_info << "First line was the shebang line, ignoring..." << std::endl;
        std::getline(source, line); // Ignore the shebang line if it exists
    }
    linelen= line.size();
    if (dev_mode)
        debug_info << "Detected playfield width 0o" << std::oct << linelen
                   << std::dec << "\nStackowey source:\n    +"
                   << n_times_char(linelen, '-') << '+' << std::endl;
    do {
        if (dev_mode)
            debug_info << "    |" << line << '|' << std::endl;
        if (line.size() != linelen)
            error_out(E_SYNTAX, "The eastward edge is too rough!");
        playfield.push_back(line);
    } while (std::getline(source, line) &&
             !(source_from_stdin && line == "---"));
    if (dev_mode)
        debug_info << "    +" << n_times_char(linelen, '-') << '+' << std::endl;
}

void close_source() {
    source_filestream.close();
}

void read_source(const char* source_path) {
    if (dev_mode)
        debug_info << "Reading source file from following location:\n    "
                   << source_path << std::endl;
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
    std::vector<char32_t> unicode_line= unicodepoints(line);
    for (char32_t character : unicode_line) {
        push_stack(static_cast<uint64_t>(character));
    }
    push_stack(0);
}

void send_output() {
    uint64_t character= pop_stack();
    if (dev_mode)
        debug_info << "Outputting U+" << std::hex << character << std::dec << std::endl;
    std::cout << utf8_encode(character % UINT32_MAX); // Modulo to prevent invalid codepoint
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
            dev_mode= true;
            debug_info.open("./swy.log");
            if (debug_info.fail()) {
                dev_mode= false;
                error_out(E_STREAM_W, "Could not open 'swy.log' in current working directory for writing!");
            }
            debug_info << "--- start of log ---\n" << std::endl;
        }
        if (argc >= 3)
            read_source(argv[2]); // FIXME: Why does this crash?
        else
            read_source_from_stream(std::cin);
    } else
        read_source(argv[1]);

    if (dev_mode)
        debug_info << "\n--- start of execution ---\n" << std::endl;

    char current_command= '9'; // Halt if nothing is given
    if (linelen == 0 || playfield.size() == 0)
        error_out(E_SYNTAX, "Gimme nuffin', get nuffin'.");
    while (true) {
        // Modulo to loop around instead of running off playfield.
        pos[0]         = pos[0] % playfield.size();
        pos[1]         = pos[1] % linelen;
        current_command= playfield[pos[0]][pos[1]];
        if (dev_mode)
            debug_info << "Moved " << direction_names[direction] << " to "
                       << get_command_name(current_command) << " at l:0o" << std::oct
                       << pos[0] << ",c:0o" << pos[1] << std::dec << std::endl;
        switch (current_command) {
            case '/': {
                uint64_t values[2]= { pop_stack(), pop_stack() };
                if (values[0] > values[1]) {
                    if (direction == RIGHT || direction == LEFT)
                        direction=
                        Direction((static_cast<uint8_t>(direction) + 1 + 4) % 4); // +4 to avoid ever going negative
                    else
                        direction=
                        Direction((static_cast<uint8_t>(direction) - 1 + 4) % 4); // +4 to avoid ever going negative
                    if (dev_mode)
                        debug_info << "Bounced off to the "
                                   << direction_names[direction] << std::endl;
                } else {
                    // Ignore this potential turn
                    if (dev_mode)
                        debug_info << "value[0]<=value[1], continuing to the "
                                   << direction_names[direction] << std::endl;
                }
                break;
            }
            case '\\': {
                uint64_t values[2]= { pop_stack(), pop_stack() };
                if (values[0] < values[1]) {
                    if (direction == RIGHT || direction == LEFT)
                        direction=
                        Direction((static_cast<uint8_t>(direction) - 1 + 4) % 4); // +4 to avoid ever going negative
                    else
                        direction=
                        Direction((static_cast<uint8_t>(direction) + 1 + 4) % 4); // +4 to avoid ever going negative
                    if (dev_mode)
                        debug_info << "Bounced off to the "
                                   << direction_names[direction] << std::endl;
                } else {
                    // Ignore this potential turn
                    if (dev_mode)
                        debug_info << "value[0]>=value[1], continuing to the "
                                   << direction_names[direction] << std::endl;
                }
                break;
            }
            case '?': {
                if (std::cin.fail())
                    error_out(E_STREAM_R, "STDIN cancelled!");
                get_input();
                break;
            }
            case '!': {
                send_output();
                break;
            }
            case '%': {
                pos[0]= pop_stack();
                pos[1]= pop_stack();
                if (dev_mode)
                    debug_info << "Teleported to l:0o" << std::oct << pos[0]
                               << ",c:0o" << pos[1] << std::dec << std::endl;
                break;
            }
            case '0': {
                push_stack(0);
                break;
            }
            case '1': {
                push_stack(1);
                break;
            }
            case '2': {
                push_stack(2);
                break;
            }
            case '3': {
                push_stack(3);
                break;
            }
            case '4': {
                push_stack(4);
                break;
            }
            case '5': {
                push_stack(5);
                break;
            }
            case '6': {
                push_stack(6);
                break;
            }
            case '7': {
                push_stack(7);
                break;
            }
            case '+': {
                push_stack(pop_stack() + pop_stack());
                break;
            }
            case '_': {
                push_stack(~pop_stack());
                break;
            }
            case '#': {
                if (stack.size() == 0)
                    break;
                uint64_t raw_i= pop_stack();
                if (raw_i >= stack.size()) {
                    // Elements below the stack are random. Swapping with a random element is almost the same as just replacing the top value with a random one.
                    pop_stack(); // This value gets swapped into the abyss and disappears
                    push_stack(random_number());
                    break;
                }
                uint64_t i= ((stack.size() - 1) - raw_i) %
                            stack.size(); // Choose a position inside the stack
                uint64_t value_i= stack[i];
                stack[i]        = pop_stack();
                push_stack(value_i);
                if (dev_mode)
                    debug_info << "Swapped 0o" << n_th(raw_i)
                               << " (from the top) and top stack element" << std::endl;
                break;
            }
            case '@': {
                uint64_t raw_i= pop_stack();
                if (raw_i >= stack.size()) {
                    push_stack(random_number()); // Copying an element from below the stack is the same as just pushing a random value
                }
                uint64_t i= ((stack.size() - 1) - raw_i) %
                            stack.size(); // Choose a position inside the stack
                push_stack(stack[i]);
                if (dev_mode)
                    debug_info << "Copy of 0o" << n_th(raw_i) << " stack element pushed to stack"
                               << std::dec << std::endl;
                break;
            }
            case '.': {
                pop_stack();
                if (dev_mode)
                    debug_info << "Popped a stack value into the abyss" << std::endl;
                break;
            }
            case '=': {
                push_stack(stack.size());
                if (dev_mode)
                    debug_info << "Pushed current stack size ("
                               << stack.size() - 1 << ") to stack" << std::endl;
                break;
            }
            case '8': {
                push_stack(pos[0]);
                push_stack(pos[1]);
                if (dev_mode)
                    debug_info << "Pushed coordinates to stack" << std::endl;
                break;
            }
            case '9': {
                if (dev_mode)
                    debug_info << "Halting!" << std::endl;
                return 0;
            }
            default: {
                // Ignore unrecognized character
            }
        }
        direction= Direction(static_cast<uint8_t>(direction) % 4);
        switch (direction) {
            case RIGHT: {
                pos[1]++; // Increment column number
                break;
            }
            case DOWN: {
                pos[0]--; // Decrement line number (Note: playfield is upside-down internally!)
                break;
            }
            case LEFT: {
                pos[1]--; // Decrement column number
                break;
            }
            case UP: {
                pos[0]++; // Increment line number (Note: playfield is upside-down internally!)
                break;
            }
            default: {
                error_out(E_3D, "I'm flat___. However, you should've come better prepared.");
            }
        }

        if (dev_mode)
            debug_info << std::endl;
    }

    return 0;
}
