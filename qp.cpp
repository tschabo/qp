#include "qp.hpp"

#include <format>
#include <string_view>

// Implement quoted-printable encoding
std::string qp::encode(const std::string &str)
{
    enum
    {
        normal,
        CR,
    } state = normal;
    std::string result;
    result.reserve(str.size() + (str.size() / 4));
    std::string currentLine;
    currentLine.reserve(76 + 5);

    const auto fnEscape = [&currentLine](char c) -> void
    {
        if (c == 9 || (c >= 32 && c <= 60) || (c >= 62 && c <= 126))
            currentLine.push_back(c);
        else
            currentLine += std::format("={:02X}", c);
    };

    const auto fnHandleLineBreaks = [&currentLine, &result]() -> void
    {
        if (currentLine.size() < 76)
        {
            if (currentLine.size() >= 2 && currentLine[currentLine.size() - 2] == '\r' && currentLine[currentLine.size() - 1] == '\n')
            {
                result += currentLine;
                currentLine.clear();
            }
            return;
        }
        std::string_view svCurrentLine(currentLine);
        const auto idx = [&]
        {
            const auto i = svCurrentLine.substr(76 - 3, 3).rfind('=');
            if (i == std::string_view::npos)
                return 2ull;
            return i;
        }();
        result += svCurrentLine.substr(0, 76 - 3 + idx);
        result += "=\r\n";
        const std::string tmp{svCurrentLine.substr(76 - 3 + idx)};
        currentLine.clear();
        currentLine += tmp;
    };

    for (const auto c : str)
    {
        
        switch (state)
        {
        case normal:
            if (c == '\r')
                state = CR;
            else
                fnEscape(c);
            break;
        case CR:
            if (c == '\n')
            {
                currentLine += "\r\n";
                state = normal;
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
                state = normal;
            }
            break;
        }
        fnHandleLineBreaks();
    }
    fnHandleLineBreaks();
    if (!currentLine.empty())
        result += currentLine;
    return result;
}