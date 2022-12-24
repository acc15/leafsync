#pragma once

#include <string>
#include <vector>

#include "index_flags.hpp"

namespace lf {

    struct index_entry {
        std::string name;
        index_flags flags;
        std::vector<index_entry> entries = {};
    };

}