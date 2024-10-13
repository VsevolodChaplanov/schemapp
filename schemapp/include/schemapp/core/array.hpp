#ifndef SCHEMAPP_CORE_ARRAY_HPP
#define SCHEMAPP_CORE_ARRAY_HPP

#include <schemapp/core/tagged.hpp>
#include <schemapp/core/tree.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {
    template<typename T, static_string Tag, tree_c Tree>
    struct array : public string_tagged<Tag>, public type_tagged<T> {
        using tree_type = Tree;
    };
} // namespace schemapp

#endif // SCHEMAPP_CORE_ARRAY_HPP