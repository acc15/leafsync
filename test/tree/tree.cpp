#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <fstream>

#include "tree/tree.hpp"
#include "tree/tree_print.hpp"
#include "tree/tree_binary.hpp"

#include "../io/log_tester.hpp"

using namespace lf;
using namespace std::filesystem;

struct bool_tree: tree<bool> {
    static const char file_signature[];
    static const uint8_t file_version;

    bool_tree(): tree() {
    }
    
    bool_tree(const bool& data, const entry_map& entries): tree {.data = data, .entries = entries} {
    }

};

const char bool_tree::file_signature[] = "TEST";
const uint8_t bool_tree::file_version = 0;

namespace lf {
    
    std::ostream& operator<<(std::ostream& s, with_format<format::BINARY, const bool&> v) {
        return s.put(v.value);
    }

    std::istream& operator>>(std::istream& s, with_format<format::BINARY, bool&> v) {
        std::uint8_t value;
        if (s >> value) {
            v.value = static_cast<bool>(value);
        }
        return s;
    }

}

const bool_tree test_tree(false, {
    { "a", tree<bool> { .data = false, .entries = {
        { "1.json", tree<bool> { .data = false } }, 
        { "2.yaml", tree<bool> { .data = false } }
    }}},
    { "b", tree<bool> {.data = false, .entries = {
        { "3.sql", tree<bool> { .data = true } }, 
        { "4.jpg", tree<bool> { .data = false } }
    }}},
    { "c", tree<bool> {.data = false, .entries = {
        { "5.cpp", tree<bool> { .data = false } }, 
        { "6.hpp", tree<bool> { .data = true } }
    }}},
    { "d", tree<bool> {.data = false, .entries = {
        { "7.txt", tree<bool> { .data = true } }, 
        { "8.bin", tree<bool> { .data = true } }
    }}},
    { "e", tree<bool> { .data = true, .entries = {
        { "9.json", tree<bool> { .data = false } }, 
        { "10.yaml", tree<bool> { .data = false } }
    }}},
    { "f", tree<bool> {.data = true, .entries = {
        { "11.sql", tree<bool> { .data = true } }, 
        { "12.jpg", tree<bool> { .data = false } }
    }}},
    { "g", tree<bool> {.data = true, .entries = {
        { "13.cpp", tree<bool> { .data = false } }, 
        { "14.hpp", tree<bool> { .data = true } }
    }}},
    { "h", tree<bool> {.data = true, .entries = {
        { "15.txt", tree<bool> { .data = true } }, 
        { "16.bin", tree<bool> { .data = true } }
    }}}
});

const path test_index_path = "b/3.sql";

TEST_CASE("print", "[tree]") {
    std::stringstream ss;
    ss << std::boolalpha << test_tree;
    REQUIRE(ss.str() == 
        "<root> [false]\n"
        "├── g [true]\n"
        "│   ├── 14.hpp [true]\n"
        "│   └── 13.cpp [false]\n"
        "├── f [true]\n"
        "│   ├── 12.jpg [false]\n"
        "│   └── 11.sql [true]\n"
        "├── e [true]\n"
        "│   ├── 10.yaml [false]\n"
        "│   └── 9.json [false]\n"
        "├── c [false]\n"
        "│   ├── 6.hpp [true]\n"
        "│   └── 5.cpp [false]\n"
        "├── h [true]\n"
        "│   ├── 16.bin [true]\n"
        "│   └── 15.txt [true]\n"
        "├── b [false]\n"
        "│   ├── 4.jpg [false]\n"
        "│   └── 3.sql [true]\n"
        "├── d [false]\n"
        "│   ├── 8.bin [true]\n"
        "│   └── 7.txt [true]\n"
        "└── a [false]\n"
        "    ├── 2.yaml [false]\n"
        "    └── 1.json [false]\n"
    );
}

template <typename T>
void cmp_tree(const tree<T>& l, const tree<T>& r) {
    {
        INFO("l=" << l.data << ",r=" << r.data);
        CHECK(l.data == r.data);
    }
    for (const auto& le: l.entries) {
        const auto rei = r.entries.find(le.first);
        INFO("node=" << le.first);
        if (rei == r.entries.end()) {
            FAIL_CHECK("missing node in r tree");
            continue;
        }
        cmp_tree(le.second, rei->second);
    }
    for (const auto& re: r.entries) {
        INFO("node=" << re.first);
        if (!l.entries.contains(re.first)) {
            FAIL_CHECK("missing node in r tree");
        }
    }
}

TEST_CASE("get", "[tree]") {
    REQUIRE(test_tree.get(test_index_path));
    REQUIRE_FALSE(test_tree.get("a/b/c"));
}

TEST_CASE("set update flags", "[tree]") {
    bool_tree tree;
    tree.set(test_index_path, true);
    REQUIRE(tree.get(test_index_path));
}

TEST_CASE("serialization", "[tree]") {
    const path path = temp_directory_path() / "tree_serialization_test.lf";
    
    std::fstream file(path, std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
    REQUIRE( file << with_ref_format<format::BINARY>(test_tree) );
    REQUIRE( file.seekg(0).good() );

    bool_tree d;
    REQUIRE( file >> with_ref_format<format::BINARY>(d) );

    cmp_tree(test_tree, d);
}

TEST_CASE("deserialize with wrong signature", "[tree]") {
    log_tester t;

    std::stringstream ss("WRONGSIGNATUREHERE!!!");

    bool_tree d;
    REQUIRE_FALSE( ss >> with_ref_format<format::BINARY>(d) );
    REQUIRE( t.str().find("invalid file signature") != std::string::npos );
}

TEST_CASE("set", "[tree]") {
    bool_tree tree;
    CHECK_FALSE( tree.set(false) );
    REQUIRE( tree.set(true) );
    CHECK_FALSE( tree.set(true) );
    REQUIRE( tree.set(false) );
}

TEST_CASE("set must avoid creating entries with default flags only", "[tree]") {
    bool_tree tree;
    REQUIRE( tree.entries.empty() );

    CHECK_FALSE( tree.set(test_index_path, false) );
    REQUIRE( tree.entries.empty() );
}

TEST_CASE("set must remove redunant entries", "[tree]") {
    bool_tree tree;

    CHECK( tree.set(test_index_path, true) );
    REQUIRE( !tree.entries.empty() );
    
    CHECK( tree.set(test_index_path, false) );
    REQUIRE( tree.entries.empty() );
}

TEST_CASE("set must preserve adjanced entries when removing redunant entries", "[tree]") {
    bool_tree tree;

    const path adjanced_path = test_index_path.parent_path() / "x.jpg";

    CHECK( tree.set(test_index_path, true) );
    CHECK( tree.set(adjanced_path, true) );
    REQUIRE( !tree.entries.empty() );
    
    CHECK( tree.set(test_index_path, false) );
    REQUIRE( tree.node(adjanced_path) != nullptr );
}

TEST_CASE("set must preserve intermediate non-default nodes", "[tree]") {
    path intermediate_path = path("a") / "b";
    path file_path = intermediate_path / "c" / "file.txt";

    bool_tree tree;
    CHECK( tree.set(intermediate_path, true) );
    CHECK( tree.set(file_path, true) );
    REQUIRE( tree.node(file_path) != nullptr );

    CHECK( tree.set(file_path, false) );
    REQUIRE( tree.node(file_path) == nullptr );

    auto intermediate_entry = tree.node(intermediate_path);
    REQUIRE( intermediate_entry != nullptr );
    REQUIRE( intermediate_entry->entries.empty() );
}

TEST_CASE("set must keep entries with children", "[tree]") {
    const path intermediate_path = "a/b";
    const path nested_path = intermediate_path / "c/test.yaml";

    bool_tree tree;
    CHECK( tree.set(intermediate_path, true) );
    CHECK( tree.set(nested_path, true) );
    REQUIRE( tree.node(nested_path) != nullptr );

    CHECK( tree.set(intermediate_path, false) );
    REQUIRE_FALSE( tree.get(intermediate_path) );
    REQUIRE( tree.node(nested_path) != nullptr );
}

TEST_CASE("set must keep entries with single children", "[tree]") {

    const path common_path = "a/b";
    const path p1 = common_path / "x/y";
    const path p2 = common_path / "z/w";

    bool_tree tree;
    CHECK( tree.set(p1, true) );
    CHECK_FALSE( tree.set(p2, false) );

    REQUIRE( tree.node(p1) != nullptr );

}
