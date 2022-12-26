#include <filesystem>
#include <string>
#include <unordered_map>
#include <istream>

#include "../io/with_format.hpp"

namespace lf {

    struct config_sync {
        std::filesystem::path local;
        std::filesystem::path remote;
        std::filesystem::path state;
        std::filesystem::path index;
    };

    using config = std::unordered_map<std::string, config_sync>;
    std::istream& operator>>(std::istream& s, with_format_and_errors<format::YAML, config&> dest);

    std::filesystem::path get_config_path();
    config load_config();

}