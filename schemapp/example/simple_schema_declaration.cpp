#include <schemapp/core/entry.hpp>
#include <schemapp/schemapp.hpp>
#include <type_traits>

using namespace schemapp;           // NOLINT(google-build-using-namespace)
using namespace schemapp::literals; // NOLINT(google-build-using-namespace)

using bool_entry = entry<struct bool_tag, "bool_key"_fs>;
using int_entry = entry<struct int_tag, "int_key"_fs>;
using double_entry = entry<struct double_tag, "double_key"_fs>;

using example_tree = tree<struct tree_tag, "tree"_fs, bool_entry, int_entry, double_entry,
                          entry<struct string_extra_entry_tag, "string_tag"_fs>>;

static_assert(std::is_same_v<typename bool_entry::tag_type, struct bool_tag>);
static_assert(bool_entry::tag_value == "bool_key"_fs);

using string_entry = example_tree::get_entry_by_tag<struct string_extra_entry_tag>;

static_assert(std::is_same_v<typename string_entry::tag_type, string_extra_entry_tag>);
static_assert(string_entry::tag_value == "string_tag"_fs);

int main() {}