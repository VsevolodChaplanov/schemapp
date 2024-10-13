#ifndef SCHEMAPP_IO_HPP
#define SCHEMAPP_IO_HPP

#include <schemapp/core/constraints.hpp>
#include <schemapp/core/entry.hpp>
#include <schemapp/utility/static_string.hpp>

namespace schemapp {

    namespace detail {
        constexpr static_string xml_bra = "<";
        constexpr static_string xml_ket = "/>"; // close xml too
        constexpr static_string space = " ";

        constexpr static_string xsd_element = "xsd:element";

        template<c_entry Entry> constexpr static_string field_name = R"(name=")" + Entry::tag_value + R"(")";

        template<typename Entry>
        concept has_type_name = requires {
            typename Entry::type;
            typename detail::stringify_helper<typename Entry::type>;
        };

        template<typename Entry> struct type_name_xml_schema {
            static constexpr static_string value = "";
        };

        template<has_type_name Entry> struct type_name_xml_schema<Entry> {
            static constexpr static_string value = R"( type=")" + detail::schema_type_tag<Entry> + R"(")";
        };

        template<typename Entry> constexpr static_string type_name_xml_schema_v = type_name_xml_schema<Entry>::value;
    } // namespace detail

    template<c_entry Entry>
    constexpr static_string entry_xml_schema_v =
        detail::xml_bra + detail::xsd_element + detail::space + detail::field_name<Entry> +
        detail::type_name_xml_schema_v<Entry> + detail::xml_ket;
} // namespace schemapp

#endif // SCHEMAPP_IO_HPP