#ifndef SCHEMAPP_CORE_TREE_HPP
#define SCHEMAPP_CORE_TREE_HPP

#include <tuple>

#include <schemapp/core/entry.hpp>
#include <schemapp/core/getters.hpp>
#include <schemapp/core/tagged.hpp>
#include <schemapp/utility/index.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {

    template<typename TypeTag, static_string Tag, entry_c... Entries>
    struct tree : public string_tagged<Tag>, type_tagged<TypeTag> {
        using entries_tuple = std::tuple<Entries...>;

        template<std::size_t Idx> using get_entry_by_idx = get_entry_by_idx<Idx, Entries...>;
        template<typename KeyTag> using get_entry_by_tag = get_entry_by_tag<KeyTag, Entries...>;
        template<static_string Key> using get_entry_by_key = get_entry_by_key<Key, Entries...>;
    };

    template<typename T>
    concept tree_c = type_tagged_c<T> && string_tagged_c<T> && requires { typename T::entries_tuple; };
} // namespace schemapp

#endif // SCHEMAPP_CORE_TREE_HPP