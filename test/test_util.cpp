#include <fmt/core.h>

#include "test_util.hpp"

namespace leafsync {

    const std::filesystem::path test_dir = std::filesystem::path(__FILE__).parent_path();
    
#ifdef _WIN32
    const std::filesystem::path test_root = "C:";
#else
    const std::filesystem::path test_root = "/";
#endif

    std::filesystem::path test_path(std::string_view p) {
        return test_root / p;
    }

    std::string test_pstr(std::string_view p) {
        return test_path(p).string();
    }

    void test_error_handler::error(std::string_view msg) {
        messages.push_back(static_cast<std::string>(msg));
        has_errors = true;
    }



}

