#include "cmd.hpp"
#include "../config/config.hpp"
#include "../index/index_sync_mode.hpp"
namespace lf {

    class add_cmd: public cmd {
    public:
        
        static const char* names[];
        static const cmd_desc desc;

        add_cmd();
        int run(const std::span<const char*>& args) const override;

        void process_path(const config& cfg, std::string_view path_str) const;
        void add_path(const config_sync& sync, const std::filesystem::path& rel_path, index_sync_mode mode) const;

    };

}