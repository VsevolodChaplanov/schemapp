#include <catch2/catch_test_macros.hpp>

#include <schemapp/core/entry.hpp>
#include <schemapp/core/getters.hpp>
#include <schemapp/core/to_string.hpp>
#include <schemapp/core/tree.hpp>
#include <schemapp/utility/index.hpp>
#include <schemapp/utility/static_string.hpp>

using namespace schemapp;           // NOLINT(google-build-using-namespace)
using namespace schemapp::literals; // NOLINT(google-build-using-namespace)

using test_entry = entry<struct any, "any">;

template<static_string String, std::size_t Indent> //
constexpr static_string add_indent = " " + add_indent<String, Indent - 1>;

template<static_string String> constexpr static_string add_indent<String, 0> = String;

TEST_CASE("static string class unit tests") {
    STATIC_CHECK(entry_xml_schema_v<entry<struct any, "any">::constrained<type_c<double>>> ==
                 R"(<xsd:element name="any" type="double"/>)");

    STATIC_CHECK(entry_xml_schema_v<test_entry> == R"(<xsd:element name="any"/>)");

    STATIC_CHECK(add_indent<entry_xml_schema_v<test_entry>, 2> == R"(  <xsd:element name="any"/>)");
}