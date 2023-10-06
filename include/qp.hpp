#pragma once


#ifdef _WIN32
#ifdef EXPORT_QP_DLL
#define QP_API __declspec(dllexport)
#else
#define QP_API
#endif
#else
#define QP_API
#endif

#include <string>

namespace qp {
    enum flags: unsigned int{
        none = 0,
        use_dot_stuffing = 1,
        throw_on_invalid_input = 2,
    };
    QP_API std::string encode(const std::string& str, flags f = flags::none);
    QP_API std::string decode(const std::string& str, flags f = flags::none);
}