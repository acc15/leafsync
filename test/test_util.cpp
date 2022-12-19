#include <fmt/core.h>

#include "test_util.hpp"

namespace leafsync {

    const std::filesystem::path test_dir = std::filesystem::path(
        std::source_location::current().file_name()
    ).parent_path();

    void test_error_handler::error(std::string_view msg) {
        messages.push_back(static_cast<std::string>(msg));
        has_errors = true;
    }
}

