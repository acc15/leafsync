#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "cmd/cmd.hpp"

using namespace lf;

class test_cmd: public cmd {
public:
    test_cmd(): cmd({ "a", "b", "c"}, "description", { {opt { "opt", 'o', "option description" }} }) {}
    int run(const opt_map&) const override {
        return 0;
    }
};

TEST_CASE("print", "[cmd_desc]") {
    test_cmd cmd;
    std::stringstream ss;
    ss << cmd;

    REQUIRE(ss.str() == 
        "a, b, c - description\n"
        "    --opt, -o - option description\n"
    );
}
