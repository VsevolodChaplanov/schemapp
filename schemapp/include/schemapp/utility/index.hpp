#ifndef SCHEMAPP_UTILITY_INDEX_HPP
#define SCHEMAPP_UTILITY_INDEX_HPP

#include <concepts>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {
    namespace detail {
        using literals::operator""_fs;

        /**
         * @brief fictive type
         * @details if we not pass extra type in index search algo we will always obtains last element
         */
        struct dummy_entry {
            using tag_type = struct not_usable_dummy_tag;
            static constexpr auto tag_value = "not_usable_dummy_tag"_fs;
        };

        /**
         * @brief search for index in pack by type tag
         *
         * @tparam Tag - searching type
         * @tparam TypeTagged - type tagged types
         * @return size_t - index in pack
         */
        template<typename Tag, typename... TypeTagged> consteval auto index_helper() noexcept -> size_t {
            std::size_t counter{0};
            std::ignore = ((counter++, std::is_same_v<Tag, typename TypeTagged::tag_type>) || ...);
            return counter - 1;
        }

        /**
         * @brief search for index in pack by string tag
         *
         * @tparam Tag - searching tag
         * @tparam StringTagged - string tagged types
         * @return size_t - index in pack
         */
        template<static_string Tag, typename... StringTagged> consteval auto index_helper() noexcept -> size_t {
            std::size_t counter{0};
            std::ignore = ((counter++, Tag == StringTagged::tag_value) || ...);
            return counter - 1;
        }
    } // namespace detail

    template<typename T>
    concept has_type_tag_c = requires { typename T::tag_type; };

    template<typename Tag, typename... TypeTagged> consteval auto index() noexcept -> size_t {
        /// Use the helper to be able to return an index outside the pack
        return detail::index_helper<Tag, TypeTagged..., detail::dummy_entry>();
    }

    template<typename T>
    concept has_value_tag_c = requires { typename std::integral_constant<decltype(T::tag_value), T::tag_value>; };

    template<static_string Tag, typename... StringTagged> consteval auto index() noexcept -> size_t {
        /// Use the helper to be able to return an index outside the pack
        return detail::index_helper<Tag, StringTagged..., detail::dummy_entry>();
    }
} // namespace schemapp

#endif // SCHEMAPP_UTILITY_INDEX_HPP