#include <catch2/catch_test_macros.hpp>
#include <schemapp/core/entry.hpp>
#include <schemapp/core/getters.hpp>
#include <schemapp/core/tree.hpp>
#include <schemapp/utility/index.hpp>
#include <schemapp/utility/static_string.hpp>
#include <type_traits>

using namespace schemapp;           // NOLINT(google-build-using-namespace)
using namespace schemapp::literals; // NOLINT(google-build-using-namespace)

TEST_CASE("static string class unit tests") {
    using namespace std::literals::string_literals;
    using namespace std::literals::string_view_literals;

    REQUIRE(std::string{"my_package"} == "my_package"_fs.view());
    REQUIRE(std::string_view{"my_package"} == "my_package"_fs.view());

    REQUIRE(std::string{"my_package"_fs} == "my_package");
    REQUIRE(std::string_view{"my_package"_fs} == "my_package");

    REQUIRE(std::string{"my_package"_fs} == std::string_view{"my_package"});
    REQUIRE(std::string_view{"my_package"_fs} == std::string_view{"my_package"});

    REQUIRE(std::string{"my_package"_fs} == std::string{"my_package"});
    REQUIRE(std::string_view{"my_package"_fs} == std::string{"my_package"});
}

using bool_entry = entry<struct bool_tag, "bool_key"_fs>;
using int_entry = entry<struct int_tag, "int_key"_fs>;
using double_entry = entry<struct double_tag, "double_key"_fs>;

TEST_CASE("indexing by tags in parameters pack tests, check indices is correct") {

    constexpr auto index_of_bool = schemapp::index<bool_tag, bool_entry, int_entry, double_entry>();
    STATIC_CHECK(index_of_bool == 0);

    constexpr auto index_of_int = schemapp::index<int_tag, bool_entry, int_entry, double_entry>();
    STATIC_CHECK(index_of_int == 1);

    constexpr auto index_of_double = schemapp::index<double_tag, bool_entry, int_entry>();
    STATIC_CHECK(index_of_double >= 2); // check index over
}

TEST_CASE("get element using string key using indexing") {
    constexpr auto index_of_bool = schemapp::index<"bool_key"_fs, bool_entry, int_entry>();
    STATIC_CHECK(index_of_bool == 0);

    constexpr auto index_of_int = schemapp::index<int_entry::tag_value, bool_entry, int_entry>();
    STATIC_CHECK(index_of_int == 1);

    constexpr auto index_of_double = schemapp::index<"double_key"_fs, bool_entry, int_entry>();
    STATIC_CHECK(index_of_double == 2); // check index over
}

TEST_CASE("indexing by tags in tuple tests, check indices is correct") {
    using bool_entry_found = get_entry_by_tag<struct bool_tag, bool_entry, int_entry, double_entry>;
    STATIC_CHECK(std::is_same_v<bool_entry_found, bool_entry>);

    using int_entry_found = get_entry_by_key<"int_key"_fs, bool_entry, int_entry, double_entry>;
    STATIC_CHECK(std::is_same_v<int_entry_found, int_entry>);

    using double_entry_found = get_entry_by_key<"double_key"_fs, bool_entry, int_entry, double_entry>;
    STATIC_CHECK(std::is_same_v<double_entry_found, double_entry>);
}

using tree_test = tree<struct tree_tag, "tree"_fs, bool_entry, int_entry, double_entry,
                       entry<struct string_extra_entry_tag, "string_tag"_fs>>;

TEST_CASE("get entries of tree_test, check correct types") {
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_idx<0>, bool_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_idx<1>, int_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_idx<2>, double_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_idx<3>::tag_type, struct string_extra_entry_tag>);

    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_tag<struct bool_tag>, bool_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_tag<struct int_tag>, int_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_tag<struct double_tag>, double_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_tag<struct string_extra_entry_tag>::tag_type,
                                struct string_extra_entry_tag>);

    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_key<"bool_key"_fs>, bool_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_key<"int_key"_fs>, int_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_key<"double_key"_fs>, double_entry>);
    STATIC_CHECK(std::is_same_v<tree_test::get_entry_by_key<"string_tag"_fs>::tag_type, struct string_extra_entry_tag>);
}