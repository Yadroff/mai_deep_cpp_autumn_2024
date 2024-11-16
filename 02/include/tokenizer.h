#pragma once

#include <functional>
#include <sstream>

class Tokenizer {
public:
    enum Tokens {
        DIGIT = 0,
        STRING,

        LAST
    };

    using StartCallback = std::function<void()>;
    using EndCallback = std::function<void()>;
    using DigitCallback = std::function<void(int)>;
    using StrCallback = std::function<void(std::string_view)>;

    Tokenizer() = default;

    void SetStartCallback(StartCallback&& cb) { onStart = cb; }
    void SetEndCallback(EndCallback&& cb) { onEnd = cb; }

    void SetOnDigit(DigitCallback&& cb) { onDigit = cb; }
    void SetOnString(StrCallback&& cb) { onStr = cb; }

    void Parse(std::istringstream& stream);
    void Parse(const std::string& str);

private:
    StartCallback onStart;
    EndCallback onEnd;
    DigitCallback onDigit;
    StrCallback onStr;
};