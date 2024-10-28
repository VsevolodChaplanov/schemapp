#ifndef SCHEMAPP_CORE_TREE_HPP
#define SCHEMAPP_CORE_TREE_HPP

#include <tuple>

#include <schemapp/core/entry.hpp>
#include <schemapp/core/getters.hpp>
#include <schemapp/core/tagged.hpp>
#include <schemapp/utility/index.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {

    namespace concepts {
        template<typename Tag, typename... Entry>
        concept tag_exist_in_entries = ((has_type_tag<Entry> && std::same_as<Tag, typename Entry::tag_type>) || ...);

        namespace detail {
            template<typename T, typename V, typename... Ts> struct types_unique_helper;

            template<has_type_tag T, has_type_tag V>
            struct types_unique_helper<T, V>
                : std::bool_constant<!std::same_as<typename T::tag_type, typename V::tag_type>> {};

            template<has_type_tag T, has_type_tag V, has_type_tag... Ts>
            struct types_unique_helper<T, V, Ts...>
                : public std::conjunction<types_unique_helper<T, V>, types_unique_helper<T, Ts...>,
                                          types_unique_helper<V, Ts...>> {
                using value_type = bool;
                static constexpr value_type value = types_unique_helper<T, V>::value &&
                                                    types_unique_helper<T, Ts...>::value &&
                                                    types_unique_helper<V, Ts...>::value;
            };

            template<has_type_tag... Ts> constexpr auto types_unique_helper_v = types_unique_helper<Ts...>::value;

            template<typename T, typename V, typename... Ts> struct tag_values_unique_helper;

            template<has_value_tag T, has_value_tag V>
            struct tag_values_unique_helper<T, V> : std::bool_constant<T::tag_value != V::tag_value> {};

            template<has_value_tag T, has_value_tag V, has_value_tag... Ts>
            struct tag_values_unique_helper<T, V, Ts...>
                : public std::conjunction<types_unique_helper<T, V>, types_unique_helper<T, Ts...>,
                                          types_unique_helper<V, Ts...>> {
                using value_type = bool;
                static constexpr value_type value = tag_values_unique_helper<T, V>::value &&
                                                    tag_values_unique_helper<T, Ts...>::value &&
                                                    tag_values_unique_helper<V, Ts...>::value;
            };

            template<has_value_tag... Ts>
            constexpr auto tag_values_unique_helper_v = tag_values_unique_helper<Ts...>::value;
        } // namespace detail

        template<typename... Ts> struct types_unique;

        template<> struct types_unique<> final : public std::bool_constant<true> {};

        template<has_type_tag T> struct types_unique<T> final : public std::bool_constant<true> {};

        template<has_type_tag T, has_type_tag V, has_type_tag... Ts>
        struct types_unique<T, V, Ts...> final : public detail::types_unique_helper<T, V, Ts...> {};

        template<has_type_tag... Ts> constexpr bool types_unique_v = types_unique<Ts...>::value;

        template<typename... Ts> struct tag_values_unique;

        template<> struct tag_values_unique<> final : public std::bool_constant<true> {};

        template<has_value_tag T> struct tag_values_unique<T> final : public std::bool_constant<true> {};

        template<has_value_tag T, has_value_tag V, has_value_tag... Ts>
        struct tag_values_unique<T, V, Ts...> final : public detail::tag_values_unique_helper<T, V, Ts...> {};

        template<has_value_tag... Ts> constexpr bool tag_values_unique_v = tag_values_unique<Ts...>::value;
    } // namespace concepts

    /**
     * @brief dictionary representation
     * @details heterogeneous collection of Entries
     * @tparam TypeTag some type tag for type resolving
     * @vparam Tag string tree tag
     * @tparam Entries... heterogeneous collection of entries in this root
     */
    template<typename TypeTag, static_string Tag, concepts::entry... Entries>
        requires(concepts::types_unique_v<Entries...> && concepts::tag_values_unique_v<Entries...>)
    struct tree : public string_tagged<Tag>, type_tagged<TypeTag> {
        // static_assertions do not work without creation class
        // instance used for "help" messages for concepts above

        static_assert(concepts::types_unique_v<Entries...>, "all struct tags in entries must be unique for one tree");
        static_assert(concepts::tag_values_unique_v<Entries...>,
                      "all tag values in entries must be unique for one tree");

        /**
         * @brief use to obtain all entries
         */
        using entries_tuple = std::tuple<Entries...>;

        /**
         * @brief get entry by its index
         */
        template<std::size_t Idx>
            requires(Idx < sizeof...(Entries))
        using get_entry_by_idx = detail::get_entry_by_idx<Idx, Entries...>;

        /**
         * @brief get entry by struct tag
         */
        template<concepts::tag_exist_in_entries<Entries...> KeyTag>
        using get_entry_by_tag = detail::get_entry_by_tag<KeyTag, Entries...>;

        /**
         * @bried get entry by string key
         */
        template<static_string Key> using get_entry_by_key = detail::get_entry_by_key<Key, Entries...>;
    };

    template<typename T>
    concept tree_c = type_tagged_c<T> && string_tagged_c<T> && requires { typename T::entries_tuple; };
} // namespace schemapp

#endif // SCHEMAPP_CORE_TREE_HPP