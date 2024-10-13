#ifndef SCHEMAPP_CORE_CONSTRAINTS_HPP
#define SCHEMAPP_CORE_CONSTRAINTS_HPP

#include <concepts>
#include <schemapp/utility/static_string.hpp>
#include <string>
#include <string_view>

namespace schemapp {

    namespace tags {
        /**
         * @brief for base classes lookup
         */

        struct min_constraint_tag {};
        struct max_constraint_tag {};
        struct default_value_constraint_tag {};
        struct enumeration_constraint_tag {};
    } // namespace tags

    template<auto Min, bool Inclusive = true> struct min_c : public tags::min_constraint_tag {
        using min_value_type = decltype(Min);
        static constexpr bool min_inclusive = Inclusive;
        static constexpr auto min = Min;
    };

    template<auto Max, bool Inclusive = false> struct max_c : public tags::max_constraint_tag {
        using max_value_type = decltype(Max);
        static constexpr auto max_inclusive = Inclusive;
        static constexpr auto max = Max;
    };

    template<auto Default> struct default_value_c : public tags::default_value_constraint_tag {
        using default_value_type = decltype(Default);
        static constexpr const auto& default_value = Default;
    };

    template<auto... Values>
        requires(sizeof...(Values) != 0)
    struct enumeration_c : public tags::enumeration_constraint_tag {
        static constexpr std::tuple enumeration{Values...};
    };

    template<typename Type> struct type_c {
        using type = Type;
    };

    template<typename Entry, typename... Constraints> struct with_constraints : public Entry, public Constraints... {};

    namespace concepts {
        template<typename T>
        concept derived_from_min_c = requires {
            typename T::min_value_type;
            { T::min_inclusive } -> std::convertible_to<bool>;
            T::min;
        } && std::derived_from<T, min_c<T::min, T::min_inclusive>> && std::derived_from<T, tags::min_constraint_tag>;

        template<typename T>
        concept derived_from_max_c = requires {
            typename T::max_value_type;
            { T::max_inclusive } -> std::convertible_to<bool>;
            T::max;
        } && std::derived_from<T, max_c<T::max, T::max_inclusive>>;

        template<typename T>
        concept derived_from_default_value_c = requires {
            typename T::default_value_type;
            T::default_value;
        } && std::derived_from<T, default_value_c<T::default_value>>;

        template<typename T>
        concept derived_from_enumeration_c =
            requires { T::enumeration; } && std::derived_from<T, tags::enumeration_constraint_tag>;
    } // namespace concepts

    namespace detail {

        // extend by specialization

        template<typename T = void> struct stringify_helper {
            static constexpr static_string type_name = "";
        };
        template<> struct stringify_helper<double> final {
            static constexpr static_string type_name = "double";
        };
        template<> struct stringify_helper<int> final {
            static constexpr static_string type_name = "int";
        };
        template<> struct stringify_helper<std::string> final {
            static constexpr static_string type_name = "string";
        };
        template<> struct stringify_helper<std::string_view> final {
            static constexpr static_string type_name = "string";
        };

        template<> struct stringify_helper<void> final {
            static constexpr static_string type_name = "";
        };

        template<typename T, typename Type = typename T::type>
        constexpr static_string schema_type_tag = stringify_helper<Type>::type_name;

        using constr = type_c<double>;
        static_assert(schema_type_tag<constr> == "double");
    } // namespace detail
} // namespace schemapp

#endif // SCHEMAPP_CORE_CONSTRAINTS_HPP