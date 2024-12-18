#include <catch2/catch_test_macros.hpp>

#include <schemapp/core/constraints.hpp>

namespace schemapp::testing {
    struct my_test_type : public default_value_c<0>, public min_c<0> {};
    struct my_test_type_2 {};

    TEST_CASE("check concepts check constraints correctly") {
        STATIC_CHECK(concepts::derived_from_default_value_c<my_test_type>);
        STATIC_CHECK(concepts::derived_from_min_c<my_test_type>);
        STATIC_CHECK_FALSE(concepts::derived_from_default_value_c<my_test_type_2>);
    }
} // namespace schemapp::testing