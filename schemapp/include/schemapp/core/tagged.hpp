#ifndef SCHEMAPP_CORE_TAGGED_HPP
#define SCHEMAPP_CORE_TAGGED_HPP

#include <schemapp/utility/static_string.hpp>

namespace schemapp {
    /**
     * @brief common ancestor for types with the string tag
     *
     * @tparam Tag - tag of field, like key in json/xml and e.t.c
     */
    template<static_string Tag> struct string_tagged {
        static constexpr auto tag_value = Tag;
    };

    template<typename StringTagged>
    concept string_tagged_c = requires {
        typename std::integral_constant<decltype(StringTagged::tag_value), StringTagged::tag_value>;
        std::ranges::size(StringTagged::tag_value) != 0;
    };

    /**
     * @brief common ancestor for types with the type tag
     *
     * @tparam Tag - tag of field, c++ type
     */
    template<typename Tag> struct type_tagged {
        using tag_type = Tag;
    };

    template<typename TypeTagged>
    concept type_tagged_c = requires { typename TypeTagged::tag_type; };
} // namespace schemapp

#endif // SCHEMAPP_CORE_TAGGED_HPP