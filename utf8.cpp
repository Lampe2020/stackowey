#include <cstdint>
#include <string>
#include <vector>

// Unicode codepoint for replacement character;
#define INVALID 0xFFFD

std::string utf8_encode(char32_t codepoint) {
    std::string result;

    if (codepoint <= 0x7F) {
        result+= static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
        result+= static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
        result+= static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        result+= static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
        result+= static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result+= static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        result+= static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
        result+= static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        result+= static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result+= static_cast<char>(0x80 | (codepoint & 0x3F));
    } else {
        result= utf8_encode(INVALID);
    }

    return result;
}

std::string from_unicodepoints(std::vector<char32_t> codepoints) {
    std::string result = "";
    for (uint64_t i=0; i<codepoints.size(); i++)
        result.append(utf8_encode(codepoints[i]));
    return result;
}

std::vector<char32_t> unicodepoints(std::string source_string) {
    std::vector<char32_t> result;
    size_t i = 0;
    while (i < source_string.size()) {
        uint8_t byte = static_cast<uint8_t>(source_string[i]);
        char32_t codepoint = 0;
        size_t remaining = 0;
        
        if (byte <= 0x7F) {
            codepoint = byte;
            remaining = 0;
        } else if ((byte >> 5) == 0b110) {
            codepoint = byte & 0x1F;
            remaining = 1;
        } else if ((byte >> 4) == 0b1110) {
            codepoint = byte & 0x0F;
            remaining = 2;
        } else if ((byte >> 3) == 0b11110) {
            codepoint = byte & 0x07;
            remaining = 3;
        } else {
            result.push_back(INVALID);
            ++i;
            continue;
        }
        
        if (i + remaining >= source_string.size()) {
            result.push_back(INVALID);
            ++i;
            continue;
        }
        
        bool valid = true;
        for (size_t j = 0; j < remaining; j++) {
            uint8_t next_byte = static_cast<uint8_t>(source_string[i + 1 + j]);
            if ((next_byte >> 6) != 0b10) {
                valid = false;
                break;
            }
            codepoint = (codepoint << 6) | (next_byte & 0x3F);
        }
        
        if (valid) {
            result.push_back(codepoint);
            i += 1 + remaining;
        } else {
            result.push_back(INVALID);
            i++;
        }
    }
    
    return result;
}

char32_t utf8_decode(const std::string& str) {
    std::vector<char32_t> decoded = unicodepoints(str);
    if (!decoded.empty()) {
        return decoded[0];
    }
    return 0;
}
