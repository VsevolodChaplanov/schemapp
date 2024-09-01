#include <iostream>
#include <schemapp/utility/static_string.hpp>

using namespace schemapp::literals;

auto main() -> int {
    constexpr auto hello_world = "hello world"_fs;
    std::cout << hello_world.view() << std::endl;
    return 0;
}
