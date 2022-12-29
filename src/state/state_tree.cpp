#include "state/state_tree.hpp"

namespace lf {

    const char* const state_tree::name = "state";
    const char state_tree::file_signature[4] = "LFS";
    const uint8_t state_tree::file_version = 0;

    std::istream& operator>>(std::istream& s, with_format<format::BINARY, bool&> data) {
        std::istream::char_type c;
        if (s >> c) {
            data.value = c != 0;
        }
        return s;
    }

    std::ostream& operator<<(std::ostream& s, const with_format<format::BINARY, const bool&> data) {
        return s.put(data.value ? 1 : 0);
    }

}