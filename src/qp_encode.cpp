#include "qp.hpp"

#include <string_view>

QP_API std::string qp::encode(const std::string &str, qp::flags f)
{
    enum
    {
        valid_unescaped,
        CR,
    } state = valid_unescaped;
    std::string result;
    result.reserve(str.size() + (str.size() / 4));
    std::string currentLine;
    currentLine.reserve(76 + 5);

    const auto fnEscape = [&currentLine](char c) -> void
    {
        if (c == 9 || (c >= 32 && c <= 60) || (c >= 62 && c <= 126))
        {
            currentLine.push_back(c);
        }
        else
        {
            currentLine.push_back('=');
            for (const auto val : {(c >> 4) & 0xF, c & 0xF})
            {
                if (val < 10)
                    currentLine.push_back('0' + val);
                else
                    currentLine.push_back('A' + (val - 10));
            }
        }
    };

    const auto fnHandleLineBreaks = [&]() -> void
    {
        if (currentLine.size() < 76)
        {
            // see RFC 5321 section 4.5.2
            if (((f & qp::flags::use_dot_stuffing) == qp::flags::use_dot_stuffing) && currentLine == ".\r\n")
            {
                result.push_back('.');
                result += currentLine;
                currentLine.clear();
            }
            else if (currentLine.size() >= 2 && currentLine[currentLine.size() - 2] == '\r' && currentLine[currentLine.size() - 1] == '\n')
            {
                result += currentLine;
                currentLine.clear();
            }
            return;
        }
        std::string_view svCurrentLine(currentLine);
        const auto idx = [&]
        {
            const auto i = svCurrentLine.substr(76 - 2, 2).rfind('=');
            if (i == std::string_view::npos)
                return static_cast<decltype(i)>(2);
            return i;
        }();
        result += svCurrentLine.substr(0, 76 - 2 + idx);
        result += "=\r\n";
        const std::string tmp{svCurrentLine.substr(76 - 2 + idx)};
        currentLine.clear();
        currentLine += tmp;
    };

    for (const auto c : str)
    {

        switch (state)
        {
        case valid_unescaped:
            if (c == '\r')
                state = CR;
            else
                fnEscape(c);
            break;
        case CR:
            if (c == '\n')
            {
                currentLine += "\r\n";
                state = valid_unescaped;
                break;
            }
            else
            {
                fnEscape('\r');
            }

            if (c == '\r')
            {
                state = CR;
            }
            else
            {
                fnEscape(c);
                state = valid_unescaped;
            }
            break;
        }
        fnHandleLineBreaks();
    }
    if (!currentLine.empty())
        result += currentLine;
    return result;
}
