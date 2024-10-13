#include "schemapp/core/constraints.hpp"
#include "schemapp/core/entry.hpp"
#include "schemapp/core/tree.hpp"
#include <schemapp/schemapp.hpp>

using namespace schemapp;

using xml_schema = tree<struct root, "root",
                        entry<struct some_tag, "some_tag">::constrained<min_c<0>, max_c<100>, default_value_c<50>>>;

auto main() -> int { return 0; }