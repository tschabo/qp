#include "qp.hpp"

#include <stdexcept>
#include <array>

namespace
{
    bool isValidUnescaped(char c)
    {
        return c == 9 || (c >= 32 && c <= 60) || (c >= 62 && c <= 126);
    }
    bool isHexDigit(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
    }
}

std::string qp::decode(const std::string &str, qp::flags f)
{
    if (str.empty())
        return str;
    const auto throwOnInvalidInput = (f & qp::flags::throw_on_invalid_input) == qp::flags::throw_on_invalid_input;
    std::string result;
    std::array<char, 3> hexBuffer{0};
    result.reserve(str.size());
    enum
    {
        valid_unescaped,
        start_escape,
        soft_line_break,
        hex,
    } state = valid_unescaped;
    for (const auto c : str)
    {
        switch (state)
        {
        case valid_unescaped:
            if (c == '=')
                state = start_escape;
            else if (isValidUnescaped(c))
                result.push_back(c);
            else
            {
                if (throwOnInvalidInput)
                    throw std::runtime_error("Invalid input");
                result.push_back(c);
            }
            break;

        case start_escape:
            if (c == '\r')
                state = soft_line_break;
            else if (isHexDigit(c))
            {
                hexBuffer[0] = c;
                state = hex;
            }
            else if (throwOnInvalidInput)
            {
                throw std::runtime_error("Invalid input");
                result.push_back(c);
                state = valid_unescaped;
            }
            break;

        case soft_line_break:
            if (c == '\n')
                state = valid_unescaped;
            else
            {
                if (throwOnInvalidInput)
                    throw std::runtime_error("Invalid input");
                result.push_back('\r');
                result.push_back(c);
                state = valid_unescaped;
            }
            break;

        case hex:
            if (isHexDigit(c))
            {
                hexBuffer[1] = c;
                result.push_back(static_cast<char>(std::stoi(hexBuffer.data(), nullptr, 16)));
                state = valid_unescaped;
            }
            else
            {
                if (throwOnInvalidInput)
                    throw std::runtime_error("Invalid input");
                result.push_back(hexBuffer.at(0));
                result.push_back(c);
                state = valid_unescaped;
            }
            break;

        default:
            throw std::runtime_error("Invalid state");
        }
    }
    return result;
}