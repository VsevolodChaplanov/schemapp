#include <schemapp/schemapp.hpp>

using schemapp::literals::operator""_fs;

using some_entry = schemapp::entry<struct some_tag, "some_tag"_fs>::template constrained<schemapp::default_value_c<50>>;

using xml_schema = schemapp::tree<struct root, "root"_fs, some_entry>;

auto main() -> int {
    static_assert(std::same_as<xml_schema::tag_type, struct root>);
    static_assert(xml_schema::tag_value == "root");
    using entry_t = xml_schema::get_entry_by_tag<struct some_tag>;
    static_assert(std::same_as<entry_t::tag_type, struct some_tag>);
    static_assert(entry_t::tag_value == "some_tag");
    static_assert(entry_t::default_value == 50);
    return 0;
}