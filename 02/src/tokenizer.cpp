#include "tokenizer.h"
#include <algorithm>

void Tokenizer::Parse(std::istringstream& stream)
{
    if (onStart) {
        onStart();
    }
    std::string val;
    Tokens token = LAST;
    static auto isDigit = [](std::string_view val) { return std::all_of(val.begin(), val.end(),
                                                         [](const char c) { return std::isdigit(c); }); };
    while (stream >> val) {
        if (isDigit(val)) {
            token = DIGIT;
        } else {
            token = STRING;
        }

        switch (token) {
        case DIGIT:
            if (onDigit) {
                onDigit(std::stoi(val));
            }
            break;
        case STRING:
            if (onStr) {
                onStr(val);
            }
            break;

        case LAST:
        default:
            throw std::runtime_error("Got unexpected token");
        }
    }

    if (onEnd) {
        onEnd();
    }
}

void Tokenizer::Parse(const std::string& str)
{
    std::istringstream stream(str);
    Parse(stream);
}
