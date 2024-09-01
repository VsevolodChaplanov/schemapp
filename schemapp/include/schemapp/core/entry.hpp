#ifndef SCHEMAPP_CORE_ENTRY_HPP
#define SCHEMAPP_CORE_ENTRY_HPP

#include <schemapp/core/tagged.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {
    template<typename Entry, auto... Constraints> struct entry_traits {};

    template<typename TypeTag, static_string Tag>
    struct entry : public string_tagged<Tag>, public type_tagged<TypeTag> {};

    template<typename T>
    concept entry_c = string_tagged_c<T> && type_tagged_c<T>;
} // namespace schemapp

#endif // SCHEMAPP_CORE_ENTRY_HPP