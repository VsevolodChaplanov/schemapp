#ifndef SCHEMAPP_CORE_GETTERS_HPP
#define SCHEMAPP_CORE_GETTERS_HPP

#include <tuple>

#include <schemapp/utility/index.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {
    template<std::size_t Idx, typename... T> using get_entry_by_idx = std::tuple_element_t<Idx, std::tuple<T...>>;

    template<typename KeyTag, typename... T> using get_entry_by_tag = get_entry_by_idx<index<KeyTag, T...>(), T...>;

    template<static_string Key, typename... T> using get_entry_by_key = get_entry_by_idx<index<Key, T...>(), T...>;
} // namespace schemapp

#endif // SCHEMAPP_CORE_GETTERS_HPP