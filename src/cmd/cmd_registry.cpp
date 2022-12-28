#include "cmd/cmd_registry.hpp"

#include <ostream>
#include <sstream>
#include <iostream>

#include "cmd/sync/sync_cmd.hpp"
#include "cmd/index/add_cmd.hpp"
#include "cmd/index/rm_cmd.hpp"

namespace lf {

const sync_cmd _sync;
const add_cmd _add;
const rm_cmd _rm;

const cmd* const cmd_registry::list[] = { 
    &_sync, 
    &_add, 
    &_rm 
};

cmd_registry::cmd_registry() {
    for (const cmd* cmd: list) {
        for (const char* name: cmd->names) {
            _names[name] = cmd;
        }
    }
}

int cmd_registry::run(std::span<const char*> args) const {
    if (args.empty()) {
        std::cout << *this;
        return 0;
    }

    const char* cmd_name = args[0];
    const auto it = _names.find(cmd_name);
    if (it == _names.end()) {
        std::cerr << "Unknown command: " << cmd_name << std::endl << std::endl << *this;
        return 1;
    }
    return it->second->run(args.subspan(1));
}

std::ostream& operator<<(std::ostream& s, const cmd_registry& r) {
    const size_t sz = std::size(r.list);
    if constexpr (sz == 0) {
        return s;
    }

    s << "Usage: " << std::endl << std::endl;
    for (const cmd* cmd: r.list) {
        s << *cmd;
    }
    return s;
}

const cmd_registry cmds;

}

