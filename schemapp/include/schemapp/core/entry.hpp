#ifndef SCHEMAPP_CORE_ENTRY_HPP
#define SCHEMAPP_CORE_ENTRY_HPP

#include <schemapp/core/constraints.hpp>
#include <schemapp/core/tagged.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {

    namespace concepts {
        /**
         * @brief concept of entry
         * @tparam T
         */
        template<typename T>
        concept entry = string_tagged_c<T> && type_tagged_c<T>;
    } // namespace concepts

    /**
     * @brief structure
     * @tparam TypeTag
     * @tparam Tag
     */
    template<typename TypeTag, static_string Tag>
    struct entry : public string_tagged<Tag>, public type_tagged<TypeTag> {
        template<typename... Constraints> using constrained = with_constraints<entry, Constraints...>;
    };

} // namespace schemapp

#endif // SCHEMAPP_CORE_ENTRY_HPP