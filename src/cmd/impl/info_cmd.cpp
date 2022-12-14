#include "cmd/impl/info_cmd.hpp"

#include <iostream>
#include <filesystem>

#include "config/config.hpp"

#include "state/state_tree.hpp"
#include "index/index_tree.hpp"

namespace fs = std::filesystem;

namespace lf {

    info_cmd::info_cmd(): cmd(
        {"i", "info"}, 
        "prints info about indexed files, their sync modes and current synchronization state",
        { opt { "index", 'I', "prints index" }, opt { "state", 'S', "prints state"} }
    ) {
    }
    
    int info_cmd::run(const opt_map& opts) const {

        const config cfg = config::load();

        bool print_index = opts.has("index");
        bool print_state = opts.has("state");
        if (!print_index && !print_state) {
            print_index = true;
            print_state = true;
        }

        const fs::path path = std::filesystem::current_path();
        const config::sync_entry_vec v = cfg.find_most_specific_local_matches(path);
        if (v.empty()) {
            return 1;
        }

        for (const config::sync_entry* p: v) {
            if (print_state) {
                print_tree<state_tree>(p->first, p->second.state);
            }
            if (print_index) {
                print_tree<index_tree>(p->first, p->second.index);
            }
        }
        return 0;
    }

}